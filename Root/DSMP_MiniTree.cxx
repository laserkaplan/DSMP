#include "DSMP/DSMP_MiniTree.h"

DSMP_MiniTree::DSMP_MiniTree(xAOD::TEvent *event, TTree *tree, TFile *file) : HelpTreeBase(event, tree, file, 1e3) {
}

DSMP_MiniTree::~DSMP_MiniTree() {
}

void DSMP_MiniTree::AddEventUser(const std::string) {
    m_tree->Branch("weight", &m_weight, "weight/F");
}

void DSMP_MiniTree::AddJetsUser(const std::string, const std::string) {
    m_tree->Branch("jet_tam", &m_jet_tam);
    m_tree->Branch("jet_tam_cal", &m_jet_tam_cal);
}

void DSMP_MiniTree::AddFatJetsUser(const std::string &, const std::string &, const std::string &) {
    m_tree->Branch("fatjet_tam", &m_fatjet_tam);
    m_tree->Branch("fatjet_tam_cal", &m_fatjet_tam_cal);
}

void DSMP_MiniTree::FillEventUser(const xAOD::EventInfo *eventInfo) {
    if (eventInfo->isAvailable<float>("weight")) m_weight = eventInfo->auxdecor<float>("weight");
}

void DSMP_MiniTree::FillJetsUser(const xAOD::Jet *jet, const std::string) {
    if (jet->isAvailable<float>("JetTrackAssistedMassUnCalibrated")) m_jet_tam.push_back(jet->getAttribute<float>("JetTrackAssistedMassUnCalibrated"));
    if (jet->isAvailable<float>("JetTrackAssistedMassCalibrated")) m_jet_tam_cal.push_back(jet->getAttribute<float>("JetTrackAssistedMassCalibrated"));
}

void DSMP_MiniTree::FillFatJetsUser(const xAOD::Jet *fatjet, const std::string &, const std::string &) {
    if (fatjet->isAvailable<float>("JetTrackAssistedMassUnCalibrated")) m_fatjet_tam.push_back(fatjet->getAttribute<float>("JetTrackAssistedMassUnCalibrated"));
    if (fatjet->isAvailable<float>("JetTrackAssistedMassCalibrated")) m_fatjet_tam_cal.push_back(fatjet->getAttribute<float>("JetTrackAssistedMassCalibrated"));
}

void DSMP_MiniTree::ClearEventUser() {
    m_weight = -999;
}

void DSMP_MiniTree::ClearJetsUser(const std::string) {
    m_jet_tam.clear();
    m_jet_tam_cal.clear();
}

void DSMP_MiniTree::ClearFatJetsUser(const std::string &, const std::string &) {
    m_fatjet_tam.clear();
    m_fatjet_tam_cal.clear();
}
