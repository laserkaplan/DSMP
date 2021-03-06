#ifndef DSMP_DAODtoMT_H
#define DSMP_DAODtoMT_H

#include <xAODAnaHelpers/Algorithm.h>
#include <xAODAnaHelpers/tools/ReturnCheck.h>

#include <DSMP/DSMP_MiniTree.h>

class DSMP_DAODtoMT : public xAH::Algorithm {
    public:
        DSMP_DAODtoMT();

        virtual EL::StatusCode setupJob(EL::Job& job);
        virtual EL::StatusCode histInitialize();
        virtual EL::StatusCode initialize();
        virtual EL::StatusCode execute();
        virtual EL::StatusCode histFinalize();

        bool m_saveJets;
        bool m_saveFatJets;
        std::string m_jetContainerName;
        std::string m_fatJetContainerName;
        std::string m_eventInfoDetailStr;
        std::string m_trigDetailStr;
        std::string m_jetDetailStr;
        std::string m_fatJetDetailStr;

        DSMP_MiniTree *m_tree; //!
        
        ClassDef(DSMP_DAODtoMT, 1);
};

#endif
