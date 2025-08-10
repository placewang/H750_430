#ifndef M_SET_H_
#define M_SET_H_

typedef struct 
{
    char ReductionRatio;
    float MinimumPrecision;
    void (*RowKnifeMotorIint)(void);
    int  (*getExeTime)(void);
    void (*getMoveDistance)(float* mdec);    
    void (*getMovePos)(int* pos);
    void (*getMoveSpeed)(int* speed);   
    void (*setEncdeVale)(int endc);
    void (*setMovePos)(float* pos);
    void (*setMoveSpeed)(int* speed);
    void (*getEncde)(int *);
    void (*getRealTimePos)(int* pos);
    void (*getRealTimeSpeed)(int* speed); 
    void (*MotorStart)(char flag);
    void (*MotorMovePos)(char cmd,int MovePos); 
    
}Model_set;


extern Model_set m_set;

#endif


