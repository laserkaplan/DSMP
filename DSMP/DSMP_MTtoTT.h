#ifndef DSMP_MTtoTT_H
#define DSMP_MTtoTT_H

#include <xAODAnaHelpers/Algorithm.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include <GoodRunsLists/GoodRunsListSelectionTool.h>
#include <PileupReweighting/PileupReweightingTool.h>

#include "TTree.h"
#include "TH1.h"

enum DSMPCategory {
    _none,
    _control,
    _VV,
    _VH,
    _tV,
    _HH,
    _tH,
    _tt
};

class DSMP_MTtoTT : public xAH::Algorithm {
    public:
        DSMP_MTtoTT();

        virtual EL::StatusCode initialize();
        virtual EL::StatusCode changeInput(bool firstFile);
        virtual EL::StatusCode execute();

        void initializeVectors();
        void initializeInTree();
        void initializeOutTree();
        void copyMetaData();
        void setSumOfWeights();

        void resetBranches();
        bool passJetTrigger(int runNumber);

        // options
        bool m_doJets;
        bool m_doFatJets;
        bool m_applyFinalWeight;
        bool m_applyGRL;
        std::string m_GRLs;
        bool m_doPRW;
        std::string m_lumiCalcFiles;
        std::string m_PRWFiles;
        int m_PRWDefaultChannel;

        // tools
        GoodRunsListSelectionTool *m_GRLTool; //!
        CP::PileupReweightingTool *m_PRWTool; //!

        // input tree branches
        int in_runNumber; //! 
        long long in_eventNumber; //!
        int in_lumiblock; //!
        float in_weight; //!
        std::vector<std::string> *in_passedTriggers; //!
        int in_nJ; //!
        std::vector<float> *in_mJ; //!
        std::vector<float> *in_ptJ; //!
        std::vector<float> *in_etaJ; //!
        std::vector<float> *in_phiJ; //!
        int in_nj; //!
        std::vector<float> *in_Ej; //!
        std::vector<float> *in_ptj; //!
        std::vector<float> *in_etaj; //!
        std::vector<float> *in_phij; //!

        // output tree branches
        TTree *m_outTree; //!
        int out_runNumber; //!
        long long out_eventNumber; //!
        int out_category; //!
        float out_weight; //!
        float out_m1; //!
        float out_pt1; //!
        float out_m2; //!
        float out_pt2; //!
        float out_mDSMP; //!
        float out_ptDSMP; //!

        // MetaData
        TH1F *m_metaData; //!

        // sum of weights
        float m_sumOfWeights; //!

        ClassDef(DSMP_MTtoTT, 1);
};

#endif
