#include <stdio.h>

int main(int argc, char *argv[])
{
	int ball[12]={5,5,5,5,5,5,5,5,5,5,5,5}; //球重量
	
	if(argv[argc-1][0]==49)	//argc argv 参数改变球重量
	{
		if(argv[argc-2][1]=='\0')
			ball[(argv[argc-2][0]-49)]=6;
		else
			ball[(argv[argc-2][1]-39)]=6;
	}
	else
	{
		if(argv[argc-2][1]=='\0')
			ball[(argv[argc-2][0]-49)]=1;
		else
			ball[(argv[argc-2][1]-39)]=1;
	}

	if((ball[0]+ball[1]+ball[2]+ball[3])==(ball[4]+ball[5]+ball[6]+ball[7]))//若第一次平
	{
		if((ball[8]+ball[9]+ball[10])==(ball[0]+ball[1]+ball[2]))	//若第二次平 
		{
			if(ball[11]>ball[0]) 		//第三次
				printf("12号球重\n");
			else 
				printf("12号球轻\n"); 
		} 
		else if((ball[8]+ball[9]+ball[10])>(ball[0]+ball[1]+ball[2]))//若第二次左重
		{
			if(ball[8]>ball[9])
				printf("9号球重\n");
			else if(ball[8]<ball[9])
				printf("10号球重\n"); 
			else
				printf("11号球重\n"); 
		} 
		else if((ball[8]+ball[9]+ball[10])<(ball[0]+ball[1]+ball[2]))//若第二次左轻
		{
			if(ball[8]<ball[9])
				printf("9号球轻\n");
			else if(ball[8]>ball[9])
				printf("10号球轻\n"); 
			else
				printf("11号球轻\n");
		} 
	} 
	else if((ball[0]+ball[1]+ball[2]+ball[3])>(ball[4]+ball[5]+ball[6]+ball[7]))//若第一次左重 
	{
		if((ball[0]+ball[5]+ball[6]+ball[7])==(ball[4]+ball[8]+ball[9]+ball[10])) //若第二次平
		{
			if(ball[1]==ball[2])
				printf("4号球重\n");
			else if(ball[1]>ball[2])
				printf("2号球重\n");
			else
				printf("3号球重\n");
		} 
		else if((ball[0]+ball[5]+ball[6]+ball[7])>(ball[4]+ball[8]+ball[9]+ball[10])) //若第二次左重
		{
			if(ball[0]==ball[8]) 
				printf("5号球轻\n");
			else if(ball[0]>ball[8])
				printf("1号球重\n");
		} 
		else//若第二次左轻
		{
			if(ball[5]==ball[6]) 
				printf("8号球轻\n");
			else if(ball[5]>ball[6])
				printf("7号球轻\n");
			else
				printf("6号球轻\n");
		} 
	} 
	else//若第一次左轻 
	{
		if((ball[4]+ball[1]+ball[2]+ball[3])==(ball[0]+ball[8]+ball[9]+ball[10])) //若第二次平
		{
			if(ball[5]==ball[6])
				printf("8号球重\n");
			else if(ball[5]>ball[6])
				printf("6号球重\n");
			else
				printf("7号球重\n");
		} 
		else if((ball[4]+ball[1]+ball[2]+ball[3])>(ball[0]+ball[8]+ball[9]+ball[10])) //若第二次左重
		{
			if(ball[4]==ball[8]) 
				printf("1号球轻\n");
			else if(ball[4]>ball[8])
				printf("5号球重\n");
		} 
		else//若第二次左轻
		{
			if(ball[1]==ball[2]) 
				printf("4号球轻\n");
			else if(ball[1]>ball[2])
				printf("3号球轻\n");
			else
				printf("2号球轻\n");
		} 
	} 
	
	return 0;	
} 