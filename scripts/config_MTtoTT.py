from xAH_config import xAH_config

c = xAH_config()

GRLList = [
    "$ROOTCOREBIN/data/DSMP/data15_13TeV.periodAllYear_DetStatus-v79-repro20-02_DQDefects-00-02-02_PHYS_StandardGRL_All_Good_25ns.xml",
    "$ROOTCOREBIN/data/DSMP/data16_13TeV.periodAllYear_DetStatus-v80-pro20-08_DQDefects-00-02-02_PHYS_StandardGRL_All_Good_25ns.xml",
]
GRLs = ','.join(GRLList)

c.setalg("DSMP_MTtoTT", { "m_doJets"            : False,
                          "m_doFatJets"         : True,
                          "m_applyFinalWeight"  : True,
                          "m_applyGRL"          : True,
                          "m_GRLs"              : GRLs,
                          "m_doPRW"             : False,
                          "m_lumiCalcFiles"     : "",
                          "m_PRWFiles"          : "",
                          "m_PRWDefaultChannel" : 0,
                        } )
