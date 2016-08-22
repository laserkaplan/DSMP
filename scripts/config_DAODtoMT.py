from xAH_config import xAH_config

c = xAH_config()

triggersList = [
    "HLT_j380",
    "HLT_j420_a10_lcw_L1J100",
    "HLT_j420_a10r_L1J100",
    "HLT_ht1000_L1J100",
]
triggers = ",".join(triggersList)

if args.is_MC:
    jet_calibSeq = "JetArea_Residual_Origin_EtaJES_GSC_JMS"
else:
    jet_calibSeq = "JetArea_Residual_Origin_EtaJES_GSC_JMS_Insitu"

c.setalg("BasicEventSelection", { "m_name"                  : "BasicEventSelection",
                                  "m_debug"                 : False,
                                  "m_derivationName"        : "JETM8Kernel",
                                  "m_applyGRLCut"           : False,
                                  "m_doPUreweighting"       : False,
                                  "m_vertexContainerName"   : "PrimaryVertices",
                                  "m_PVNTrack"              : 2,
                                  "m_applyPrimaryVertexCut" : True,
                                  "m_applyEventCleaningCut" : True,
                                  "m_applyCoreFlagsCut"     : True,
                                  "m_triggerSelection"      : triggers,
                                  "m_storeTrigDecisions"    : True,
                                  "m_applyTriggerCut"       : False,
                                  "m_useMetaData"           : True
                                } )

c.setalg("JetCalibrator", { "m_name"                    : "JetCalibrator",
                            "m_inContainerName"         : "AntiKt4EMTopoJets",
                            "m_jetAlgo"                 : "AntiKt4EMTopo",
                            "m_outputAlgo"              : "AntiKt4EMTopo_Calib_Algo",
                            "m_outContainerName"        : "CalibJets",
                            "m_debug"                   : False,
                            "m_verbose"                 : False,
                            "m_sort"                    : True,
                            "m_saveAllCleanDecisions"   : True,
                            "m_calibConfigFullSim"      : "JES_2015dataset_recommendation_May2016_JMS.config",
                            "m_calibConfigData"         : "JES_2015dataset_recommendation_May2016_JMS.config",
                            "m_calibSequence"           : jet_calibSeq,
                            "m_setAFII"                 : False,
                            "m_JESUncertConfig"         : "$ROOTCOREBIN/data/JetUncertainties/JES_2015/ICHEP2016/JES2015_SR_Scenario1.config",
                            "m_JESUncertMCType"         : "MC15",
                            "m_JERUncertConfig"         : "JetResolution/Prerec2015_xCalib_2012JER_ReducedTo9NP_Plots_v2.root",
                            "m_JERFullSys"              : False,
                            "m_JERApplyNominal"         : False,
                            "m_redoJVT"                 : False,
                            "m_systName"                : "Nominal",
                            "m_systVal"                 : 0,
                          } )

c.setalg("JetSelector", { "m_name"                    : "JetSelector",
                          "m_inContainerName"         : "CalibJets",
                          "m_inputAlgo"               : "AntiKt4EMTopo_Calib_Algo",
                          "m_outContainerName"        : "SelJets",
                          "m_outputAlgo"              : "SelJets_Algo",
                          "m_decorateSelectedObjects" : False,
                          "m_createSelectedContainer" : True,
                          "m_cleanJets"               : True,
                          "m_pT_min"                  : 200e3,
                          "m_eta_max"                 : 2.4,
                          "m_pass_min"                : 0.1,
                          "m_useCutFlow"              : True,
                          "m_doBTagCut"               : False,
                          "m_doJVF"                   : False,
                          "m_doJVT"                   : True,
                          "m_pt_max_JVT"              : 60e3,
                          "m_eta_max_JVT"             : 2.4,
                          "m_JVTCut"                  : 0.59
                        } )

c.setalg("JetCalibrator", { "m_name"                  : "FatJetCalibrator",
                            "m_inContainerName"       : "AntiKt10LCTopoTrimmedPtFrac5SmallR20Jets",
                            "m_jetAlgo"               : "AntiKt10LCTopoTrimmedPtFrac5SmallR20",
                            "m_outputAlgo"            : "AntiKt10LCTopoTrimmedPtFrac5SmallR20_Calib_Algo",
                            "m_outContainerName"      : "CalibFatJets",
                            "m_debug"                 : False,
                            "m_verbose"               : False,
                            "m_sort"                  : True,
                            "m_saveAllCleanDecisions" : True,
                            "m_calibConfigFullSim"    : "JES_MC15recommendation_FatJet_track_assisted_January2016.config",
                            "m_calibConfigData"       : "JES_MC15recommendation_FatJet_track_assisted_January2016.config",
                            "m_doCleaning"            : False,
                            "m_JESUncertConfig"       : "$ROOTCOREBIN/data/JetUncertainties/UJ_2015/ICHEP2016/HbbTagging_strong.config",
                            "m_JESUncertMCType"       : "MC15C",
                            "m_calibSequence"         : "EtaJES_JMS",
                            "m_setAFII"               : False,
                            "m_jetCleanCutLevel"      : "LooseBad",
                            "m_jetCleanUgly"          : True,
                            "m_cleanParent"           : True,
                            "m_applyFatJetPreSel"     : True,
                            "m_systName"              : "Nominal",
                            "m_systVal"               : 0
                          } )

c.setalg("JetSelector", { "m_name"                    : "FatJetSelector",
                          "m_inContainerName"         : "CalibFatJets",
                          "m_inputAlgo"               : "AntiKt10LCTopoTrimmedPtFrac5SmallR20_Calib_Algo",
                          "m_outContainerName"        : "SelFatJets",
                          "m_outputAlgo"              : "SelFatJets_Algo",
                          "m_decorateSelectedObjects" : False,
                          "m_createSelectedContainer" : True,  
                          "m_cleanJets"               : True,
                          "m_pT_min"                  : 200e3,
                          "m_eta_max"                 : 2.0,
                          "m_mass_min"                : 0.1, 
                          "m_useCutFlow"              : True,
                          "m_doJVF"                   : False
                        } )

c.setalg("DSMP_DAODtoMT", { "m_name"                 : "DSMP_DAODtoMT",
                            "m_jetContainerName"     : "SelJets",
                            "m_fatJetContainerName"  : "SelFatJets",
                            "m_eventInfoDetailStr"   : "pileup",
                            "m_jetDetailStr"         : "kinematic",
                            "m_fatJetDetailStr"      : "kinematic substructure"
                          } )
