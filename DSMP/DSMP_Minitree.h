#ifndef DSMP_Minitree_H
#define DSMP_Minitree_H

#include "xAODAnaHelpers/HelpTreeBase.h"

#include "xAODEventInfo/EventInfo.h"

class DSMP_Minitree : public HelpTreeBase {
    private:
        float m_weight;
        std::vector<float> m_jet_tam;
        std::vector<float> m_jet_tam_cal;
        std::vector<float> m_fatjet_tam;
        std::vector<float> m_fatjet_tam_cal;

    public:
        DSMP_Minitree(xAOD::TEvent *event, TTree *tree, TFile *file);
        ~DSMP_Minitree();

        void AddEventUser(const std::string detailStr = "");
        void AddJetsUser(const std::string detailStr = "", const std::string jetName = "jet");
        void AddFatJetsUser(const std::string detailStr = "");
        void FillEventUser(const xAOD::EventInfo *eventInfo);
        void FillJetsUser(const xAOD::Jet *jet, const std::string jetName = "jet");
        void FillFatJetsUser(const xAOD::Jet *fatjet);
        void ClearEventUser();
        void ClearJetsUser(const std::string jetName = "jet");
        void ClearFatJetsUser();
};

#endif
