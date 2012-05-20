/*
 * sim.c
 * how to run this program:
 *	cut -f5 SSSSSS.TXT | ./sim | more
 * Command Line:
 *	cut -f5 601628.TXT | ./sim_fee -c0.999
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#define STOP_THRESHOLD	(0.98)
#define START_POINT (100000)	// one hundred thousand RMB 100,000
#define HAS_STOCK	(1)
#define HAS_MONEY	(2)

float yesterday_close_price = 0.0;	// history price
float today_close_price = 0.0;		// current price
float diff = 0.0;
int inc_or_dec = 0;			// flag indicating increase/decrease
float stop_point = 0.0;			// stop loss point
float stock_number_cnt = 0.0;		// number of stock i have
float stock_number_money = 0.0;		// sum of money i have
int has_stock = 0;			// flag indicating i have stock/money
int op_cnt = 0;
float stop_threshold = STOP_THRESHOLD;

int
save_data_to_env (float y_close_price,
	float t_close_price,
	float diff,
	int inc_or_dec, 
	float s_point,
	float s_number_cnt,
	float s_number_money,
	int has_stock,
	int op_cnt,
	float stop_threshold)
{
	char buffer [64];
	char *p = 0;

	/* putenv */	
	sprintf (buffer, "%f",  y_close_price);
	if (p = getenv ("YY_CLOSE_PRICE"))
	{
		printf ("myYY_CLOSE_PRICE=(%s)\n", p);
	}


	if (setenv ("YY_Y_CLOSE_PRICE", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%f",  t_close_price);
	if (setenv ("YY_T_CLOSE_PRICE", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%f",  diff);
	if (setenv ("YY_DIFF", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%d",  inc_or_dec);
	if (setenv ("YY_INC_OR_DEC", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%f",  s_point);
	if (setenv ("YY_S_POINT", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%f",  s_number_cnt);
	if (setenv ("YY_S_NUMBER_CNT", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%f",  s_number_money);
	if (setenv ("YY_S_NUMBER_MONEY", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%d",  has_stock);
	if (setenv ("YY_HAS_STOCK", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%d",  op_cnt);
	if (setenv ("YY_OP_CNT", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	sprintf (buffer, "%f",  stop_threshold);
	if (setenv ("YY_STOP_THRESHOLD", buffer, 1))
	{
		printf ("setenv error\n");
		return -4;
	}

	return 0;
}

int
main (int argc, char ** argv)
{
	float temp = 0;
	float oldpoint = 0;
	float data = 0.0;
	int i = 0;
	int c = 0;



	while ((c = getopt (argc, argv, "abc:")) != -1)
		switch (c)
		{
			case 'a':
				break;
			case 'b':
				break;
			case 'c':
				if (0 == atof (optarg))
				{ 
					printf ("command args error\n");	
					return 0;
				}
				stop_threshold = atof (optarg);
				printf ("stop_threshold=(%f)", stop_threshold);
				if (stop_threshold > 1)
				{
					printf ("stop_threshold error\n");
					return -2;
				}
				break;
			case '?':
				return ;
				break;
			default:
				printf ("can NOT understand command arg(s)\n");
				return;
		}

	/*
	 * make the initial investation at the very begining.
	 */
	if (EOF != scanf ("%f", &data))
	{
		yesterday_close_price = data;
		stop_point = yesterday_close_price * stop_threshold;
		stock_number_cnt = START_POINT / yesterday_close_price; //buy in
		stock_number_money = 0;	// we have no money now
		has_stock = HAS_STOCK;
		op_cnt ++;
		printf ("buy in at price(%5f), cnt(%5f)\n",
			yesterday_close_price,
			stock_number_cnt);
	} else {
		return -1;
	}


	// we read from standard input
	while (EOF != scanf ("%f", &data))
	{
		today_close_price = data;
		diff = fabsf (today_close_price - yesterday_close_price);
		inc_or_dec = ((today_close_price-yesterday_close_price)>0)?
		 		1 : 0;
		printf ("today_close_price=(%10f)inc(%d)diff(%8f)stop(%5f)\n", 
			data, inc_or_dec, diff, stop_point);
		switch (has_stock)
		{
			case HAS_STOCK:
			if (inc_or_dec ==  0 && today_close_price <= stop_point)
			{	
				// sell out
				stock_number_money =stop_point
					* stock_number_cnt;
				stock_number_cnt = 0;
				has_stock = HAS_MONEY;
				printf ("=====>\n");
				printf ("=====>sell out at price\t(%5f)\n", 
					stop_point);
				printf ("=====>\n");
				printf ("now wehas stockcnt(%10f),money(%5f)\n", 
				stock_number_cnt, stock_number_money);

				save_data_to_env (yesterday_close_price,
					today_close_price,
					diff,
					inc_or_dec, 
					stop_point,
					stock_number_cnt,
					stock_number_money,
					has_stock,
					op_cnt,
					stop_threshold);

				yesterday_close_price = today_close_price;
				continue;
			} 
			
			if (inc_or_dec) {
				oldpoint = stop_point;
				temp = today_close_price * stop_threshold;
				if (stop_point < temp)
				{
					// when the price increase,hold stock
					stop_point = today_close_price 
						* stop_threshold;
					printf ("-------move stop_point to (%f)the old is(%f)\n", 
						stop_point, oldpoint);
				}
				yesterday_close_price = today_close_price;
				continue;
			} 
			// when the price increase, hold the stock
			stock_number_money = 0;
			// stock_number_cnt NOT change
			has_stock = HAS_STOCK;
			yesterday_close_price = today_close_price;
			break;


			case HAS_MONEY:
				if (today_close_price > yesterday_close_price)
				{	
					stop_point = today_close_price 
						* stop_threshold;
					stock_number_cnt = stock_number_money 
						/ today_close_price;
					stock_number_money =  0;
					printf ("<=====\n");
					printf ("<=====buy in at price\t(%5f) with stoppoint(%f)\n", 
						today_close_price, stop_point); 
					printf ("<=====\n");
					has_stock = HAS_STOCK;
					op_cnt ++;
				}
				yesterday_close_price = today_close_price;
				break;
			default:
				// nothing todo, hold my money
				break;
		}
	}
	
	printf ("#########################################################\n");
	printf ("#########################################################\n");
	printf ("#########################################################\n");
	printf ("#########################################################\n");
	printf ("stock cnt\t(%15f), \ncntmoney\t(%15f)\nmoney\t\t(%15f)\nrate\t\t(%15f)\n", 
		stock_number_cnt, 
		stock_number_cnt * today_close_price, 
		stock_number_money,
		stop_threshold);
	printf ("#########################################################\n");
	printf ("#########################################################\n");
	printf ("#########################################################\n");

	return 0;
}

