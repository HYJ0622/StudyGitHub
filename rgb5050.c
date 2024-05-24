#if defined(RGB_LED_IC_FUN_EN)

#define RGB_NUMBER     13

u8 rgb_breath_lev[RGB_NUMBER][3];

static bool my_5050_on = 1;

u8  rgb5050_mode = 0;

u8  APS_rgb5050_mode = 0;

static bool rgb5050_mode_updata = 0;

#if defined(TWS_RGB_MODE_SYNCHRONIZATION)
    bool tws_synchronization_flag = 0;
#endif

extern void clock_add_set(u32 type);

#define rgb_set_h()		gpio_set_output_value(RGB_LED_IC_PORT,1)
#define rgb_set_l()		gpio_set_output_value(RGB_LED_IC_PORT,0)
#define rgb_set_dir()	gpio_set_direction(RGB_LED_IC_PORT,0)
#define rgb_set_hd()	gpio_set_hd(RGB_LED_IC_PORT, 0)
#define rgb_set_up()	gpio_set_pull_up(RGB_LED_IC_PORT,1)
#define rgb_set_down()	gpio_set_pull_down(RGB_LED_IC_PORT,0)

void RGB_LED_IC_PORT_OUT() //AT(.common)
{
	rgb_set_up();
	rgb_set_down();
	rgb_set_dir();
	rgb_set_hd();
}

void rgb_1_h_delay() //AT(.common)
{
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
}

void rgb_1_l_delay() //AT(.common)
{
	asm("nop"); asm("nop");
}

void rgb_0_h_delay() //AT(.common)
{
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop");
}

void rgb_0_l_delay() //AT(.common)
{
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
}

void rgb_reset_delay(u32 d) //AT(.common)
{
	while(d--){
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
		asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop"); asm("nop");
	}
}

void rgb_led_reset(void) //AT(.common)
{
   rgb_set_l();
   rgb_reset_delay(3000);
}

void rgb_led_ic_send_1byte(void) //AT(.common)
{
   rgb_set_h();
   rgb_1_h_delay();
   rgb_set_l();
   rgb_1_l_delay();
}

void rgb_led_ic_send_0byte(void) //AT(.common)
{
   rgb_set_h();
   rgb_0_h_delay();
   rgb_set_l();
   rgb_0_l_delay();
}



void rgb_led_ic_sendbyte(u8 byte) //AT(.common)
{
	if(byte & BIT(7)){
		rgb_led_ic_send_1byte();
   	}else{
  		rgb_led_ic_send_0byte();
   	}
 	if(byte & BIT(6)){
		rgb_led_ic_send_1byte();
   	}else{
  		rgb_led_ic_send_0byte();
   	}
 	if(byte & BIT(5)){
		rgb_led_ic_send_1byte();
   	}else{
  		rgb_led_ic_send_0byte();
   	}
 	if(byte & BIT(4)){
		rgb_led_ic_send_1byte();
   	}else{
  		rgb_led_ic_send_0byte();
   	}
 	if(byte & BIT(3)){
		rgb_led_ic_send_1byte();
   	}else{
  		rgb_led_ic_send_0byte();
   	}
 	if(byte & BIT(2)){
		rgb_led_ic_send_1byte();
   	}else{
  		rgb_led_ic_send_0byte();
   	}
 	if(byte & BIT(1)){
		rgb_led_ic_send_1byte();
   	}else{
  		rgb_led_ic_send_0byte();
   	}
  	if(byte & BIT(0)){
		rgb_led_ic_send_1byte();
   	}else{
  		rgb_led_ic_send_0byte();
   	}
}

void menset_rgb_breath_lev(void)
{
	for(int j=0;j<RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = 0;
		}
	}
}

static u8 rgb_spi_dat_buf[24 * RGB_NUMBER] __attribute__((aligned(4)));

#if defined(RGB5050_RUN_SPI_EN)

void rgb_led_ic_port_init(void) //AT(.common)
{
	led_spi_init();
	rgb_set_l();
	RGB_LED_IC_PORT_OUT();
}

void led5050_update(void)
{
	rgb_led_reset();
	for(int j=0;j<RGB_NUMBER;j++){
		led_spi_rgb_to_24byte(rgb_breath_lev[j][0], rgb_breath_lev[j][1], rgb_breath_lev[j][2], rgb_spi_dat_buf, j);
	}
	led_spi_send_rgbbuf_isr(rgb_spi_dat_buf, RGB_NUMBER);
	menset_rgb_breath_lev();
	rgb_set_h();
}

#else

void rgb_led_ic_port_init(void) //AT(.common)
{
	rgb_set_l();
	RGB_LED_IC_PORT_OUT();
}

void led5050_update(void)
{
	OS_ENTER_CRITICAL();
	local_irq_disable();
	clock_add_set(DEC_APE_CLK);
	rgb_led_ic_sendbyte(0);
	rgb_led_reset();
	for(int j=0;j<RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_led_ic_sendbyte(rgb_breath_lev[j][i]);
		}
	}
	OS_EXIT_CRITICAL();
	local_irq_enable();
	menset_rgb_breath_lev();
	clock_add_set(DEC_APE_CLK);
	rgb_set_h();
}

#endif


//==================================================================================================//
//												USER												//
//==================================================================================================//
void rgb5050_all_set(u8 r,u8 g,u8 b)
{
	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
            if(i == 0){
                rgb_breath_lev[j][i] = r;
            }else if(i == 1){
                rgb_breath_lev[j][i] = g;
            }else if(i == 2){
                rgb_breath_lev[j][i] = b;
            }
		}
	}
}


#define BREATH_MODE_TIME 3
#define BREATH_MODE_COLOR_SPECIES 6
void rgb5050_breath_mode(void)
{
	static u16 CNT;
	static u8 flag = 1;
	static u8 color_mode;
	static u8 breath_temp;
	static u8 rgb_buff[RGB_NUMBER][3];
	static u8 color_buff[BREATH_MODE_COLOR_SPECIES][3] = {{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1}};
	CNT++;
	if(CNT > BREATH_MODE_TIME){
		CNT = 0;
		if(flag){
			if(breath_temp < 250){
				breath_temp += 5;
			}else{
				flag = 0;
			}
		}else{
			if(breath_temp >= 5){
				breath_temp -= 5;
			}else{
				flag = 1;
				color_mode++;
				if(color_mode > BREATH_MODE_COLOR_SPECIES - 1){
					color_mode = 0;
				}
			}
		}
	}

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			if(i == 0 && color_buff[color_mode][i] == 1){
				rgb_buff[j][i] = breath_temp;
			}else if(i == 1 && color_buff[color_mode][i] == 1){
				rgb_buff[j][i] = breath_temp;
			}else if(i == 2 && color_buff[color_mode][i] == 1){
				rgb_buff[j][i] = breath_temp;
			}
		}
	}

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[j][i];
		}
	}
}

#define RUN_MODE_TIME 3
#define RUN_MODE_COLOR_SPECIES 6
void rgb5050_run_mode(void)
{
	static u16 CNT;
	static u8 color_mode;
	static u8 led_num;
	static u8 last;
	static u8 rgb_buff[RUN_MODE_COLOR_SPECIES][3] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,0,255},{0,255,255}};

	CNT++;
	if(CNT > RUN_MODE_TIME){
		CNT = 0;
		if(led_num < RGB_NUMBER - 1){
			led_num++;
		}else{
			led_num = 0;
			color_mode++;
			if(color_mode > RUN_MODE_COLOR_SPECIES - 1){
				color_mode = 0;
			}
		}
	}

	for(int j = 0;j < RGB_NUMBER;j++){
		if(j<=led_num){
			last = color_mode + 1;
			if(last >= RUN_MODE_COLOR_SPECIES){
				last = 0;
			}
			for(int i = 0;i < 3;i++){
				rgb_breath_lev[j][i] = rgb_buff[last][i];
			}
		}else{
			for(int i = 0;i < 3;i++){
				rgb_breath_lev[j][i] = rgb_buff[color_mode][i];
			}
		}
	}
}




static void (*rgb_mode_name)(void *param);//定义地址NULL的指针函数指向需要跑的函数

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//													单色灯												//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
void *rgb_spectrum_mode_r(void)
{
	static u8 rgb_buff[1][3] = {{255,0,0}};

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[0][i];
		}
	}
}

void *rgb_spectrum_mode_g(void)
{
	static u8 rgb_buff[1][3] = {{0,255,0}};

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[0][i];
		}
	}
}

void *rgb_spectrum_mode_b(void)
{
	static u8 rgb_buff[1][3] = {{0,0,255}};

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[0][i];
		}
	}
}

void *rgb_spectrum_mode_rb(void)
{
	static u8 rgb_buff[1][3] = {{255,0,255}};

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[0][i];
		}
	}
}

void *rgb_spectrum_mode_gb(void)
{
	static u8 rgb_buff[1][3] = {{0,255,255}};

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[0][i];
		}
	}
}

void *rgb_spectrum_mode_rg(void)
{
	static u8 rgb_buff[1][3] = {{255,255,0}};

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[0][i];
		}
	}
}

void *rgb_spectrum_mode_rgb(void)
{
	static u8 rgb_buff[1][3] = {{255,255,255}};

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[0][i];
		}
	}
}

void *rgb_spectrum_mode_rgb_off(void)
{
	static u8 rgb_buff[1][3] = {{0,0,0}};

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[0][i];
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//													跑马灯												//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#define MODE100_TIME 12
#define MODE100_COLOR_MODE 6
void *rgb_spectrum_mode100(void)
{
	static u16 CNT;
	static u8 color_mode;
	static u8 led_num;
	static u8 last;
	static u8 rgb_buff[MODE100_COLOR_MODE][3] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,0,255},{0,255,255}};
	#if defined(TWS_RGB_MODE_SYNCHRONIZATION)
		if(tws_synchronization_flag){
			tws_synchronization_flag = 0;
			CNT = 0;
			color_mode = 0;
			led_num = 0;
			last = 0;
		}
		if(rgb5050_mode_updata){
			rgb5050_mode_updata = 0;
			CNT = 0;
			color_mode = 0;
			led_num = 0;
			last = 0;
		}
	#endif
	CNT++;
	if(CNT > MODE100_TIME){
		CNT = 0;
		if(led_num < RGB_NUMBER - 1){
			led_num++;
		}else{
			led_num = 0;
			color_mode++;
			if(color_mode > MODE100_COLOR_MODE - 1){
				color_mode = 0;
			}
		}
	}

	for(int j = 0;j < RGB_NUMBER;j++){
		if(j<=led_num){
			last = color_mode + 1;
			if(last >= MODE100_COLOR_MODE){
				last = 0;
			}
			for(int i = 0;i < 3;i++){
				rgb_breath_lev[j][i] = rgb_buff[last][i];
			}
		}else{
			for(int i = 0;i < 3;i++){
				rgb_breath_lev[j][i] = rgb_buff[color_mode][i];
			}
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//													呼吸灯												//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#define MODE101_TIME 10
#define MODE101_COLOR_MODE 6
void *rgb_spectrum_mode101(void)
{
	static u16 CNT;
	static u8 flag = 1;
	static u8 color_mode;
	static u8 breath_temp;
	static u8 rgb_buff[RGB_NUMBER][3];
	static u8 color_buff[MODE101_COLOR_MODE][3] = {{1,0,0},{0,1,0},{0,0,1},{1,1,0},{1,0,1},{0,1,1}};
	#if defined(TWS_RGB_MODE_SYNCHRONIZATION)
		if(tws_synchronization_flag){
			tws_synchronization_flag = 0;
			CNT = 0;
			color_mode = 0;
			flag = 1;
			breath_temp = 0;
		}
		if(rgb5050_mode_updata){
			rgb5050_mode_updata = 0;
			CNT = 0;
			color_mode = 0;
			flag = 1;
			breath_temp = 0;
		}
	#endif

	CNT++;
	if(CNT > MODE101_TIME){
		CNT = 0;
		if(flag){
			if(breath_temp < 250){
				breath_temp += 5;
			}else{
				flag = 0;
			}
		}else{
			if(breath_temp >= 5){
				breath_temp -= 5;
			}else{
				flag = 1;
				color_mode++;
				if(color_mode > MODE101_COLOR_MODE - 1){
					color_mode = 0;
				}
			}
		}
	}

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			if(i == 0 && color_buff[color_mode][i] == 1){
				rgb_buff[j][i] = breath_temp;
			}else if(i == 1 && color_buff[color_mode][i] == 1){
				rgb_buff[j][i] = breath_temp;
			}else if(i == 2 && color_buff[color_mode][i] == 1){
				rgb_buff[j][i] = breath_temp;
			}
		}
	}

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i=0;i<3;i++){
			rgb_breath_lev[j][i] = rgb_buff[j][i];
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//													幻彩灯												//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#define MODE102_TIME 30
#define RGB_COLOR_NUMBER 48	//更改这个还要更改下面存颜色的数据
#if 1 //灯光颜色的方向 1:向上 0:向下
#define RGB_COLOR_DIRECTION 48
#else
#define RGB_COLOR_DIRECTION 0
#endif
void *rgb_spectrum_mode102(void)
{
	static u16 cnt,cnt1 = RGB_COLOR_DIRECTION;
	static u8 breath_temp;
	static bool r,g,b,flag;
	static u8 rgb_buff[RGB_COLOR_NUMBER][3];

	#if defined(TWS_RGB_MODE_SYNCHRONIZATION)
		if(tws_synchronization_flag){
			tws_synchronization_flag = 0;
			cnt = 0;
			cnt1 = RGB_COLOR_DIRECTION;
			breath_temp = 0;
		}
		if(rgb5050_mode_updata){
			rgb5050_mode_updata = 0;
			cnt = 0;
			cnt1 = RGB_COLOR_DIRECTION;
			breath_temp = 0;
		}
	#endif
	for(int j = 0;j < RGB_COLOR_NUMBER;j++){
		switch(j/16){
			case 0:r=1;g=1;b=0;flag=0;
				break;
	 		case 1:r=0;g=1;b=1;flag=0;
				break;
	 		case 2:r=1;g=0;b=1;flag=1;
				break;
		}
		breath_temp=255-(j%16)*15;
		for(int i = 0;i < 3;i++){
			if(((i == 0) && r)||((i == 1) && g)||((i == 2) && b)){
				if(!flag){
					rgb_buff[j][i] = breath_temp;
					breath_temp = 255 - breath_temp;
				}else{
					breath_temp = 255 - breath_temp;
					rgb_buff[j][i] = breath_temp;
				}
			}else{
				rgb_buff[j][i] = 0;
			}
		}
	}

	if(cnt < MODE102_TIME){
		cnt++;
	}else{
		cnt = 0;
		if(RGB_COLOR_DIRECTION){
			cnt1--;
			if(cnt1 == 0){
				cnt1 = RGB_COLOR_DIRECTION;
			}
		}else{
			cnt1++;
			if(cnt1 > RGB_COLOR_NUMBER){
				cnt1 = 0;
			}
		}
	}

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i = 0;i < 3;i++){
			if(cnt1+j < RGB_COLOR_NUMBER){
				rgb_breath_lev[j][i] = rgb_buff[cnt1+j][i];
			}else{
				rgb_breath_lev[j][i] = rgb_buff[cnt1+j-RGB_COLOR_NUMBER][i];
			}
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//												单色跳变灯												//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#define MODE103_TIME 300
#define MODE103_COLOR_MODE 6
void *rgb_spectrum_mode103(void)
{
	static u16 CNT;
	static u8 color_mode;
	static u8 rgb_buff[MODE103_COLOR_MODE][3] = {{255,0,0},{0,255,0},{0,0,255},{255,255,0},{255,0,255},{0,255,255}};
	#if defined(TWS_RGB_MODE_SYNCHRONIZATION)
		if(tws_synchronization_flag){
			tws_synchronization_flag = 0;
			CNT = 0;
			color_mode = 0;
		}
		if(rgb5050_mode_updata){
			rgb5050_mode_updata = 0;
			CNT = 0;
			color_mode = 0;
		}
	#endif

	CNT++;
	if(CNT > MODE103_TIME){
		CNT = 0;
		color_mode++;
		if(color_mode > MODE103_COLOR_MODE - 1){
			color_mode = 0;
		}
	}

	for(int j = 0;j < RGB_NUMBER;j++){
		for(int i = 0;i < 3;i++){
			rgb_breath_lev[j][i] = rgb_buff[color_mode][i];
		}
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//												矩阵数组												//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
u8 reflash_15v_8a[15][8]={0};

const void set_reflash_15v_8a(void)
{
	for(int i=0;i<15;i++){
		for(int j = 0;j < 8;j++){
			reflash_15v_8a[i][0]=15*j+i;
		}
	}
}








//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//										LUMINANCE_REGULATE_CTRL											//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

#if defined(RGB5050_LUMINANCE_REGULATE_EN)
void luminance_regulate_ctrl_up()
{
	switch(APS_rgb5050_mode){
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:

			break;
	}
}

void luminance_regulate_ctrl_down()
{
	switch(APS_rgb5050_mode){
		case 0:

			break;
		case 1:

			break;
		case 2:

			break;
		case 3:

			break;
		case 4:

			break;
		case 5:

			break;
		case 6:

			break;
	}
}

#endif



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//												MODE_CTRL												//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

void APS_rgbmode(void)
{
	if(rgb_mode_name){
		rgb_mode_name(NULL);
	}
}

void power_tone_stu_change(void (*function)(void *param))//获取改变扫描函数位置 //tone_play_rgbmode  tone_end_rgbmode
{
	if(function == rgb_spectrum_mode_rgb_off){
		rgb_mode_name = rgb_spectrum_mode_r;
	}else{
		rgb_mode_name = function;
	}
}

void APS_rgb5050_mode_change()
{
	switch(APS_rgb5050_mode){
		case 0:
			power_tone_stu_change(rgb_spectrum_mode100);
		break;
		case 1:
			power_tone_stu_change(rgb_spectrum_mode101);
		break;
		case 2:
			power_tone_stu_change(rgb_spectrum_mode102);
		break;
		case 3:
			power_tone_stu_change(rgb_spectrum_mode103);
		break;
		case 4:
			power_tone_stu_change(rgb_spectrum_mode103);
		break;
	}
	printf("power_tone_stu_change_mode========%d\n",APS_rgb5050_mode);
}

void APS_rgb5050_mode_ctrl()
{
	APS_rgb5050_mode++;
	#if defined(TWS_RGB_MODE_SYNCHRONIZATION)
		rgb5050_mode_updata = 1;
	#endif
	if(APS_rgb5050_mode > 3){
		APS_rgb5050_mode = 0;
	}
	APS_rgb5050_mode_change();
}

void tws_sync_rgbmode(u8 mode)
{
	APS_rgb5050_mode = mode;
	APS_rgb5050_mode_change();
}

void rgb5050_sw()
{
	my_5050_on = !my_5050_on;
}

void rgb5050_main()
{
	if(!my_5050_on){
		rgb5050_all_set(0,0,0);
		goto RGB_SCAN_END;
	}

	switch(rgb5050_mode){
		case APS_RGB:
			APS_rgbmode();
			break;
		case BREATH_RGB:
			rgb5050_breath_mode();
			break;
	}
	RGB_SCAN_END:
	led5050_update();
}

void rgb5050_main_mode(u8 mode)
{
	rgb5050_mode = mode;
}

#if defined(RGB5050_DATA_STORE_VM_EN)
void rgb5050_data_memory_write()
{
	syscfg_write(CFG_USER_RGB5050_MODE, &rgb5050_mode, 1);
	syscfg_write(CFG_USER_APS_RGB5050_MODE, &APS_rgb5050_mode, 1);
}

void rgb5050_data_memory_read()
{
	syscfg_read(CFG_USER_RGB5050_MODE, &rgb5050_mode, 1);
	syscfg_read(CFG_USER_APS_RGB5050_MODE, &APS_rgb5050_mode, 1);
}
#endif

u16 rgb5050_timer = 0;
void rgb5050_scan_enable()
{
    if (rgb5050_timer == 0) { //开启中断
    	rgb5050_timer = sys_timer_add(NULL, rgb5050_main, 5);
    }
}


#endif
