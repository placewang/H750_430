#ifndef __APP_FONTTEXT_H
#define __APP_FONTTEXT_H

#ifdef __cplusplus
extern "C" {
#endif

 
extern const char *cfgText[];
extern const char *SupgradeText[];

enum cfg_font
{
    cfgFt_OK=0,
    cfgFt_Cancel,
    cfgFt_SrtUpgrad,
    cfgFt_UpMtr,
    cfgFt_DownMtr,
    cfgFt_NULL,  
    /********************************/
    cfgFt_COUNT  // 自动计算文本总数    
    
};

enum supgr_font
{
    supgr_RestartPower=0,
    supgr_RestartSys,
    supgr_OK,
    supgr_Cancel,
};



#ifdef __cplusplus
}
#endif

#endif

