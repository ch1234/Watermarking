#define EXIT_FAILURE 0
#include "stdafx.h"

void LoadTrainingSetNoMeanStage1();
void LBG_GenerateCodebookForStageOneSize16(void);
void VQ_Stage1NoMeanSize16(void);

void calcBlockMeans(void);
void calcBlockVariances(void);
void calcPolarities(void);

void GenerateErrorImages(void);

void LoadTSNoMeanStage2(void);
void LBG_GenerateCodebookForStageTwoSize256(void);

void VQ_Stage2NoMeanEmbedding(void);

void VQ_Stage2NoMeanExtracting(void);
void ExtractSemiFragileWatermark(void);

void AddingQuantisedImages(void);

void mse_mono(void);
void NormalisedCrossCorrelation(void);




