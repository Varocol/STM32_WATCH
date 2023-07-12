#include "beep.h"

// 歌曲队列，用于存放当前播放音乐的数据
queue<pwm_frame> song_data;

// 新的频率表
uint16_t freq_tab_new[12];

// beep_tim_counter初始化结构体
TIM_TimeBaseInitTypeDef beep_tim_counter_TimeBaseInitstructure;

// beep_tim_output初始化结构体
TIM_TimeBaseInitTypeDef beep_tim_output_TimeBaseInitstructure;
TIM_OCInitTypeDef beep_tim_output_OCInitStructure;

/**
 * @brief       蜂鸣器获取歌曲长度
 * @param       song        音乐结构体
 * @retval      歌曲长度
 */
uint16_t beep_song_get_len(const struct beep_song *song)
{
    uint16_t cnt = 0;
    /* 歌曲以0x00 0x00结尾 检测结束标志*/
    while (song->data[cnt])
    {
        cnt += 2;
    }
    return cnt;
}

/**
 * @brief       蜂鸣器获取歌曲名字
 * @param       song        音乐结构体
 * @param       name        用于读取音乐名的字符数组
 * @retval      None
 */
void beep_song_get_name(const struct beep_song *song, char *name)
{
    int i = 0;
    while (song->name[i])
    {
        name[i] = song->name[i];
        i++;
    }
    name[i] = '\0';
}

/**
 * @brief       蜂鸣器调整音乐曲调
 * @param       signature|调号(0-11)       :  是指乐曲升多少个半音演奏;
 * @param       octachord|升降八度(-2到+2) :  < 0 降几个八度; > 0 升几个八度
 * @retval      None
 */
void beep_song_decode_new_freq(uint8_t signature, int8_t octachord)
{
    uint8_t i, j;
    for (i = 0; i < 12; i++) // 根据调号及升降八度来生成新的频率表
    {
        j = i + signature;

        if (j > 11) // 升完之后超出本组基本音级，就跳到下一组音级
        {
            j = j - 12;
            freq_tab_new[i] = freq_tab[j] * 2;
        }
        else
        {
            freq_tab_new[i] = freq_tab[j];
        }

        /* 升降八度 */
        if (octachord < 0)
        {
            freq_tab_new[i] >>= (-octachord);
        }
        else if (octachord > 0)
        {
            freq_tab_new[i] <<= octachord; // 每升一个八度 频率就翻一倍
        }
    }
}

/**
 * @brief       蜂鸣器音乐解码
 * @param       tone            音调
 * @param       length          音符长度
 * @param       Basicspeed      歌曲速度
 * @param       data            歌曲信息数组(用于存放解码后的信息)
 * @retval      None
 */
void beep_song_decode(uint16_t tone, uint16_t length, uint16_t Basicspeed, struct beep_song_data *data)
{
    static const uint16_t div0_len = 60.0 / Basicspeed * 1000; // 全音符的长度(ms)
    uint16_t note_len, note_sound_len, current_freq;
    uint8_t note, sharp, range, note_div, effect, dotted;

    note = tone % 10;                                        // 计算出音符
    range = tone / 10 % 10;                                  // 计算出高低音
    sharp = tone / 100;                                      // 计算出是否升半
    current_freq = freq_tab_new[sign_tab[note - 1] + sharp]; // 查出对应音符的频率

    if (note != 0)
    {
        if (range == 1)
            current_freq >>= 1; // 低音 降八度
        if (range == 3)
            current_freq <<= 1; // 高音 升八度
        data->freq = current_freq;
    }
    else
    {
        data->freq = 0;
    }
    note_div = length_tab[length % 10]; // 算出是几分音符
    effect = length / 10 % 10;          // 算出音符类型(0普通1连音2顿音)
    dotted = length / 100;              // 算出是否附点
    note_len = div0_len / note_div;     // 算出音符的时长

    if (dotted == 1)
        note_len = note_len + note_len / 2;

    if (effect != 1)
    {
        if (effect == 0) // 算出普通音符的演奏长度
        {
            note_sound_len = note_len * SOUND_SPACE;
        }
        else // 算出顿音的演奏长度
        {
            note_sound_len = note_len / 2;
        }
    }
    else // 算出连音的演奏长度
    {
        note_sound_len = note_len;
    }
    if (note == 0)
    {
        note_sound_len = 0;
    }
    data->sound_len = note_sound_len;
    data->nosound_len = note_len - note_sound_len; // 算出不发音的长度
}

/**
 * @brief       蜂鸣器获取音乐解码信息
 * @param       song            歌曲信息
 * @param       index           音符当前所在位置
 * @param       data            歌曲信息数组(用于存放解码后的信息)
 * @retval      None
 */
void beep_song_get_data(const struct beep_song *song, uint16_t index, struct beep_song_data *data)
{
    beep_song_decode(song->data[index * 2], song->data[index * 2 + 1], song->Basicspeed, data);
}

/**
 * @brief       加载音乐
 * @param       song        音乐结构体
 * @retval      None
 */
void beep_load_music(const struct beep_song *song)
{
    /* user app entry */
    struct beep_song_data data;
    int len, i = 0;
    char name[SONG_NAME_LENGTH_MAX + 1];
    /*配置乐曲曲调,song->tone = 0, 默认为C调*/
    beep_song_decode_new_freq(song->tone, song->octachord);
    /*提取乐曲名*/
    beep_song_get_name(song, name);
    /*输出乐曲名*/
    printf("[beep]正在播放：%s\n", name);
    /*获取乐曲长度*/
    len = beep_song_get_len(song);
    /*清空数据*/
    song_data = queue<pwm_frame>();
    /*解码音乐*/
    for (int i = 0; i < len; i++)
    {
        /* 解码音乐数据 */
        beep_song_get_data(song, i, &data);
        /* 装入音符有声帧*/
        song_data.push({data.freq, data.sound_len});
        /* 装入音符无声帧 */
        song_data.push({0, data.nosound_len});
    }
}

/**
 * @brief       设置蜂鸣器频率和占空比
 * @param       frame        蜂鸣器PWM帧
 * @retval      None
 */
void beep_set(pwm_frame frame)
{
    // 无声帧
    // 设置beep_tim_output
    if (frame.freq == 0)
    {
        beep_tim_output_TimeBaseInitstructure.TIM_Period = 0;
    }
    else
    {
        beep_tim_output_TimeBaseInitstructure.TIM_Period = RCC_Clocks.SYSCLK_Frequency / ((beep_tim_output_TimeBaseInitstructure.TIM_Prescaler + 1) * frame.freq) - 1; // ARR寄存器的值
    }
    TIM_TimeBaseInit(beep_tim_output, &beep_tim_output_TimeBaseInitstructure);
    beep_tim_output_OCInitStructure.TIM_Pulse = (beep_volume * 1.0 / 1000) * (beep_tim_output->ARR + 1) - 1; // 设置TIM的CCRx
    switch (beep_tim_output_channel)
    {
    case 1:
        TIM_OC1Init(beep_tim_output, &beep_tim_output_OCInitStructure);
        break;
    case 2:
        TIM_OC2Init(beep_tim_output, &beep_tim_output_OCInitStructure);
        break;
    case 3:
        TIM_OC3Init(beep_tim_output, &beep_tim_output_OCInitStructure);
        break;
    case 4:
        TIM_OC4Init(beep_tim_output, &beep_tim_output_OCInitStructure);
        break;
    default:
        return;
    }
    // 设置beep_tim_counter
    beep_tim_counter_TimeBaseInitstructure.TIM_Period = (frame.len / 1000.0) * (RCC_Clocks.SYSCLK_Frequency * 1.0 / (beep_tim_counter_TimeBaseInitstructure.TIM_Prescaler + 1));
    TIM_TimeBaseInit(beep_tim_counter, &beep_tim_counter_TimeBaseInitstructure);
}

/**
 * @brief       开启蜂鸣器
 * @retval      None
 */
void beep_on()
{
    // 开启 beep_tim_output RCC
    if (beep_tim_output == TIM1)
    {
        RCC_APB2PeriphClockCmd(beep_tim_output_rcc, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(beep_tim_output_rcc, ENABLE);
    }
    // 开启 beep_tim_counter RCC
    if (beep_tim_counter == TIM1)
    {
        RCC_APB2PeriphClockCmd(beep_tim_counter_rcc, ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(beep_tim_counter_rcc, ENABLE);
    }
    // TIM寄存器复位
    TIM_DeInit(beep_tim_output);
    TIM_DeInit(beep_tim_counter);
    // 直接进入中断
    beep_tim_counter_TimeBaseInitstructure.TIM_Period = 0;
    // 配置基本参数
    TIM_TimeBaseInit(beep_tim_counter, &beep_tim_counter_TimeBaseInitstructure);
    // 开启beep_tim_output
    TIM_Cmd(beep_tim_output, ENABLE);
    // 开启beep_tim_counter
    TIM_Cmd(beep_tim_counter, ENABLE);
    // 开启beep_tim_counter中断
    TIM_ITConfig(beep_tim_counter, TIM_IT_Update, ENABLE);
}

/**
 * @brief       关闭蜂鸣器
 * @retval      None
 */
void beep_off()
{
    // 关闭 beep_tim_output RCC
    if (beep_tim_output == TIM1)
    {
        RCC_APB2PeriphClockCmd(beep_tim_output_rcc, DISABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(beep_tim_output_rcc, DISABLE);
    }
    // 关闭 beep_tim_counter RCC
    if (beep_tim_counter == TIM1)
    {
        RCC_APB2PeriphClockCmd(beep_tim_counter_rcc, DISABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(beep_tim_counter_rcc, DISABLE);
    }
    // 关闭beep_tim_output
    TIM_Cmd(beep_tim_output, DISABLE);
    // 关闭beep_tim_counter
    TIM_Cmd(beep_tim_counter, DISABLE);
}

/**
 * @brief       蜂鸣器提示音
 * @retval      None
 */
void beep_tip()
{
    beep_music(&tip_tone);
}

/**
 * @brief       播放蜂鸣器音乐
 * @retval      None
 */
void beep_music(const struct beep_song *song)
{
    // 关闭蜂鸣器
    beep_off();
    // 加载乐谱
    beep_load_music(song);
    // 打开蜂鸣器
    beep_on();
}

/**
 * @brief       蜂鸣器中断初始化
 * @retval      None
 */
void beep_interrupt_init()
{
    // 配置中断向量
    nvic_config(NV_TIM1_UP);
}

/**
 * @brief       蜂鸣器中断服务函数
 * @retval      None
 */
void beep_interrupt_function()
{
    pwm_frame frame = song_data.front();
    song_data.pop();
    beep_set(frame);
    if (song_data.empty())
    {
        beep_off();
    }
}

/**
 * @brief  蜂鸣器初始化
 * @retval None
 */
void beep_init()
{
    // 引脚复用
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    // 开启RCC
    RCC_APB2PeriphClockCmd(beep_rcc, ENABLE);
    // 初始化引脚
    GPIO_InitTypeDef beep_gpio_initstructure = {
        .GPIO_Pin = beep_pin,
        .GPIO_Speed = GPIO_Speed_50MHz,
        .GPIO_Mode = GPIO_Mode_AF_PP,
    };
    GPIO_Init(beep_port, &beep_gpio_initstructure);
    // 初始化结构体
    // beep_tim_counter初始化结构体
    beep_tim_counter_TimeBaseInitstructure = {
        .TIM_Prescaler = (uint16_t)(RCC_Clocks.SYSCLK_Frequency / 2000 - 1), // 1999(0.5ms)
        .TIM_CounterMode = TIM_CounterMode_Up,
        .TIM_Period = 0,                   // 直接进入中断
        .TIM_ClockDivision = TIM_CKD_DIV1, // 输入信号滤波频率
        .TIM_RepetitionCounter = 0         // 重复次数,只有高级定时器才有
    };

    // beep_tim_output初始化结构体
    beep_tim_output_TimeBaseInitstructure = {
        .TIM_Prescaler = (uint16_t)(RCC_Clocks.SYSCLK_Frequency / 1000000 - 1), // 71(1us)
        .TIM_CounterMode = TIM_CounterMode_Up,
        .TIM_ClockDivision = TIM_CKD_DIV1, // 输入信号滤波频率
        .TIM_RepetitionCounter = 0         // 重复次数,只有高级定时器才有
    };
    beep_tim_output_OCInitStructure = {
        .TIM_OCMode = TIM_OCMode_PWM1,
        .TIM_OutputState = TIM_OutputState_Enable, // 使能TIM通道
    };
}