#include "DSMP/DSMP_Minitree.h"

DSMP_Minitree::DSMP_Minitree(xAOD::TEvent *event, TTree *tree, TFile *file) : HelpTreeBase(event, tree, file, 1e3) {
}

DSMP_Minitree::~DSMP_Minitree() {
}

void DSMP_Minitree::AddEventUser(const std::string) {
    m_tree->Branch("weight", &m_weight, "weight/F");
}

void DSMP_Minitree::AddJetsUser(const std::string, const std::string) {
    m_tree->Branch("jet_tam", &m_jet_tam);
    m_tree->Branch("jet_tam_cal", &m_jet_tam_cal);
}

void DSMP_Minitree::AddFatJetsUser(const std::string) {
    m_tree->Branch("fatjet_tam", &m_fatjet_tam);
    m_tree->Branch("fatjet_tam_cal", &m_fatjet_tam_cal);
}

void DSMP_Minitree::FillEventUser(const xAOD::EventInfo *eventInfo) {
    if (eventInfo->isAvailable<float>("weight")) m_weight = eventInfo->auxdecor<float>("weight");
}

void DSMP_Minitree::FillJetsUser(const xAOD::Jet *jet, const std::string) {
    if (jet->isAvailable<float>("JetTrackAssistedMassUnCalibrated")) m_jet_tam.push_back(jet->getAttribute<float>("JetTrackAssistedMassUnCalibrated"));
    if (jet->isAvailable<float>("JetTrackAssistedMassCalibrated")) m_jet_tam_cal.push_back(jet->getAttribute<float>("JetTrackAssistedMassCalibrated"));
}

void DSMP_Minitree::FillFatJetsUser(const xAOD::Jet *fatjet) {
    if (fatjet->isAvailable<float>("JetTrackAssistedMassUnCalibrated")) m_fatjet_tam.push_back(fatjet->getAttribute<float>("JetTrackAssistedMassUnCalibrated"));
    if (fatjet->isAvailable<float>("JetTrackAssistedMassCalibrated")) m_fatjet_tam_cal.push_back(fatjet->getAttribute<float>("JetTrackAssistedMassCalibrated"));
}

void DSMP_Minitree::ClearEventUser() {
    m_weight = -999;
}

void DSMP_Minitree::ClearJetsUser(const std::string) {
    m_jet_tam.clear();
    m_jet_tam_cal.clear();
}

void DSMP_Minitree::ClearFatJetsUser() {
    m_fatjet_tam.clear();
    m_fatjet_tam_cal.clear();
}
