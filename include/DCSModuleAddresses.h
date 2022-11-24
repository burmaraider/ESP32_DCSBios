#pragma once
#include <map>
  /*************************************
   * List of modules addresses
  *************************************/
  
  #define NO_MODULE         0x0000
  #define VNAO_Ready_Room   0x0500
  #define NS430             0x0600
  #define SuperCarrier      0x0700
  #define A_10C             0x1000  
  #define F_14B             0x1200
  #define UH1H              0x1400
  #define Mig19             0x1600
  #define Ka50              0x1800
  #define Yak_52            0x2000
  #define Mig21             0x2200
  #define Mig15             0x2400
  #define Mi_8MT            0x2600
  #define ChristenEagle     0x2800
  #define F_86              0x3000
  #define L_39ZA            0x3200
  #define FW190D9           0x3400
  #define C_101CC           0x3600
  #define FW190A8           0x3800
  #define I_16              0x4000
  #define Bf109k4           0x4200
  #define F_16              0x4400
  #define AJS37             0x4600
  #define JF_17             0x4800
  #define P_51D             0x5000
  #define P_47D             0x5200
  #define SpitfireLFMkIX    0x5400
  #define Edge540           0x5600
  #define SA342M            0x5800
  #define FC3               0x6000
  #define AH_6J             0x6200  
  #define Alphajet          0x6400
  #define F_22A             0x6600  
  #define Mi_24P            0x6800
  #define MosquitoFBMkVI    0x7000
  #define M2000C            0x7200
  #define FA_18C_hornet     0x7400
  #define F_5E_3            0x7600
  #define AV8BNA            0x7800
  #define AH_64D            0x8000
  #define MB_339PAN         0x8200
  #define A_4E_C            0x8400
  #define Hercules          0x8600
  #define A_29B             0x8800  
  #define T_45              0x9000  

  /// @brief A map of module addresses to module names
  /// @details This map is used to convert a module address to a module name, simply easier to read than a bunch of #defines
  const std::map<std::string, uint32_t> ModuleList
  {
    {"NO_MODULE", NO_MODULE},
    {"VNAO-Ready-Room", VNAO_Ready_Room},
    {"NS430", NS430},
    {"SuperCarrier", SuperCarrier},
    {"A-10C", A_10C},
    {"F-14B", F_14B},
    {"UH1H", UH1H},
    {"Mig19", Mig19},
    {"Ka50", Ka50},
    {"Yak-52", Yak_52},
    {"Mig21", Mig21},
    {"Mig15", Mig15},
    {"Mi-8MT", Mi_8MT},
    {"ChristenEagle", ChristenEagle},
    {"F-86", F_86},
    {"L-39ZA", L_39ZA},
    {"FW190D9", FW190D9},
    {"C-101CC", C_101CC},
    {"FW190A8", FW190A8},
    {"I-16", I_16},
    {"Bf109k4", Bf109k4},
    {"F-16", F_16},
    {"AJS37", AJS37},
    {"JF-17", JF_17},
    {"P-51D", P_51D},
    {"P-47D", P_47D},
    {"SpitfireLFMkIX", SpitfireLFMkIX},
    {"Edge540", Edge540},
    {"SA342M", SA342M},
    {"FC3", FC3},
    {"AH-6J", AH_6J},
    {"Alphajet", Alphajet},
    {"F-22A", F_22A},
    {"Mi-24P", Mi_24P},
    {"MosquitoFBMkVI", MosquitoFBMkVI},
    {"M2000C", M2000C},
    {"FA-18C-hornet", FA_18C_hornet},
    {"F-5E-3", F_5E_3},
    {"AV8BNA", AV8BNA},
    {"AH-64D", AH_64D},
    {"MB-339PAN", MB_339PAN},
    {"A-4E-C", A_4E_C},
    {"Hercules", Hercules},
    {"A-29B", A_29B},
    {"T-45", T_45}};