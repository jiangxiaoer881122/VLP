#include <stdio.h>
#include <stdint.h>
int main()
{
    //用于解析数据包是否是对的问题
    unsigned char num[]={0x28, 0x4A ,0x52, 0xA8, 0xAF, 0x2D, 0x0B, 0x82, 0xF4 ,0xC2 ,0x31, 0xCC, 0xB0, 0x00 ,0x51, 0x01 ,0xE1};
    // printf("this is \n");
    int time[10];
    int i=0;
    int a=0;
    // for(i=0;i<5;i++)
    // {

    // }
    //时间戳解析
    //总的10位被拆成了什么 ：8+2(高)
    time[0]=(num[0]<<2)|(num[1]>>6);
    //6（低）+4（高)
    time[1]=((num[1]&0x3F)<<4)|((num[2]>>4)&0x0F);
    //4（低）+6（高）
    time[2]=((num[2]&0x0F)<<6)|((num[3]>>2)&0x3F);
    //2(低)+8（高）
    time[3]=((num[3]&0x03)<<8)|num[4];
    //第二组
    a=5;
    time[0+a-1]=(num[0+a]<<2)|(num[1+a]>>6);
    //6（低）+4（高)
    time[1+a-1]=((num[1+a]&0x3F)<<4)|((num[2+a]>>4)&0x0F);
    //4（低）+6（高）
    time[2+a-1]=((num[2+a]&0x0F)<<6)|((num[3+a]>>2)&0x3F);
    //2(低)+8（高）
    time[3+a-1]=((num[3+a]&0x03)<<8)|num[4+a];
    
    //第三组
    a=a+5;
    time[0+a-2]=(num[0+a]<<2)|(num[1+a]>>6);
    //6（低）+4（高)
    time[1+a-2]=((num[1+a]&0x3F)<<4)|((num[2+a]>>4)&0x0F);

    for(i=0;i<10;i++)
    {
        printf("%d,",time[i]);        
    }
    printf("\n");
    //上面是时间戳不看
    //下面进行imu的数据解析,一共是180个 十组，每组18个
    // unsigned char imu[]={ 0x00, 0x51, 0x01, 0xE1, 0x20, 0x79, 0xFF, 0xBB, 0xFF, 0xE1,
    //             0x00, 0x07, 0xF9, 0x76, 0x04, 0x3B, 0xF4, 0xCE, 0x00, 0x4B,
    //             0x01, 0xE6, 0x20, 0x79, 0xFF, 0xBB, 0xFF, 0xE2, 0x00, 0x06,
    //             0xF9, 0x87, 0x04, 0x3B, 0xF4, 0xC8, 0x00, 0x4B, 0x01, 0xE9,
    //             0x20, 0x79, 0xFF, 0xBE, 0xFF, 0xE4, 0x00, 0x05, 0xF9, 0x83,
    //             0x04, 0x25, 0xF4, 0xBC, 0x00, 0x4C, 0x01, 0xE3, 0x20, 0x78,
    //             0xFF, 0xBE, 0xFF, 0xE5, 0x00, 0x04, 0xF9, 0x46, 0x04, 0x3D,
    //             0xF4, 0xCF, 0x00, 0x4D, 0x01, 0xE6, 0x20, 0x7C, 0xFF, 0xBB,
    //             0xFF, 0xE3, 0x00, 0x07, 0xF9, 0x70, 0x04, 0x3D, 0xF4, 0xC6,
    //             0x00, 0x4F, 0x01, 0xEA, 0x20, 0x78, 0xFF, 0xBA, 0xFF, 0xE2,
    //             0x00, 0x06, 0xF9, 0x66, 0x04, 0x47, 0xF4, 0xCB, 0x00, 0x4B,
    //             0x01, 0xEB, 0x20, 0x78, 0xFF, 0xB9, 0xFF, 0xE2, 0x00, 0x08,
    //             0xF9, 0x6E, 0x04, 0x39, 0xF4, 0xBC, 0x00, 0x48, 0x01, 0xED,
    //             0x20, 0x7C, 0xFF, 0xB5, 0xFF, 0xE1, 0x00, 0x09, 0xF9, 0x64,
    //             0x04, 0x42, 0xF4, 0xD5, 0x00, 0x46, 0x01, 0xE7, 0x20, 0x7F,
    //             0xFF, 0xB6, 0xFF, 0xE4, 0x00, 0x08, 0xF9, 0x4B, 0x04, 0x2F,
    //             0xF4, 0xFC, 0x00, 0x43, 0x01, 0xE1, 0x20, 0x76, 0xFF, 0xB7,
    //             0xFF, 0xE4, 0x00, 0x06, 0xF9, 0x5D, 0x04, 0x30, 0xF4, 0xDF};
    uint8_t imu[]={   0xFF, 0x7C, 0xFF, 0xF8, 0x20, 0x69, 0xFF, 0xF2, 0x00, 0x02,
                            0xFF, 0xDE, 0xF0, 0x00, 0x0D, 0x22, 0xD8, 0xA3, 0xFF, 0x85,
                            0xFF, 0xF5, 0x20, 0x6B, 0xFF, 0xF4, 0x00, 0x01, 0xFF, 0xE1,
                            0xF0, 0x05, 0x0D, 0x31, 0xD8, 0x83, 0xFF, 0x7F, 0xFF, 0xF6,
                            0x20, 0x6B, 0xFF, 0xFA, 0x00, 0x03, 0xFF, 0xE2, 0xF0, 0x05,
                            0x0D, 0x34, 0xD8, 0x8E, 0xFF, 0x82, 0xFF, 0xF4, 0x20, 0x5F,
                            0xFF, 0xF3, 0x00, 0x03, 0xFF, 0xE0, 0xF0, 0x0C, 0x0D, 0x3F,
                            0xD8, 0x7C, 0xFF, 0x7F, 0xFF, 0xF4, 0x20, 0x67, 0xFF, 0xF2,
                            0x00, 0x04, 0xFF, 0xE2, 0xF0, 0x11, 0x0D, 0x17, 0xD8, 0x9F,
                            0xFF, 0x7F, 0xFF, 0xFC, 0x20, 0x6B, 0xFF, 0xF1, 0x00, 0x03,
                            0xFF, 0xDF, 0xF0, 0x11, 0x0D, 0x26, 0xD8, 0x80, 0xFF, 0x88,
                            0xFF, 0xF8, 0x20, 0x67, 0xFF, 0xF1, 0x00, 0x07, 0xFF, 0xE5,
                            0xF0, 0x01, 0x0D, 0x13, 0xD8, 0x6A, 0xFF, 0x83, 0xFF, 0xF8,
                            0x20, 0x64, 0xFF, 0xF5, 0x00, 0x07, 0xFF, 0xE3, 0xF0, 0x0B,
                            0x0D, 0x36, 0xD8, 0x94, 0xFF, 0x79, 0xFF, 0xF9, 0x20, 0x65,
                            0xFF, 0xFC, 0x00, 0x04, 0xFF, 0xE2, 0xF0, 0x0E, 0x0D, 0x1D,
                            0xD8, 0x95, 0xFF, 0x7F, 0xFF, 0xFB, 0x20, 0x67, 0xFF, 0xF3,
                            0x00, 0x0A, 0xFF, 0xE3, 0xF0, 0x08, 0x0D, 0x0F, 0xD8, 0xB0
};


    int16_t imu_p[10][9]={0};
    int k=0,m=0;
    for(k=0;k<10;k++)
    {
        //imu
        for(m=0;m<6;m++)
        {
            imu_p[k][m]=(imu[k*18+2*m]<<8)+imu[k*18+2*m+1];
            printf("%d,",imu_p[k][m]);
        }
        //磁力计
        for(m=6;m<9;m++)
        {
            imu_p[k][m]=(imu[k*18+2*m]<<8)+imu[k*18+2*m+1];
            printf("%d,",imu_p[k][m]);
        }
        printf("\n");
    }
    // for(k=0;k<10;k++)
    // {
    //     //imu
    //     for(m=0;m<6;m++)
    //     {
    //         imu_p[k][m]=imu[k*18+2*m]<<8+imu[k*18+2*m+1];
    //         printf("%d,",imu_p[k][m]);
    //     }
    //     //磁力计
    //     for(m=6;m<9;m++)
    //     {
    //         imu_p[k][m]=imu[k*18+2*m]<<8+imu[k*18+2*m+1];
    //         printf("%d,",imu_p[k][m]);
    //     }
    //     printf("\n");
    // }
    
    return 0;
}