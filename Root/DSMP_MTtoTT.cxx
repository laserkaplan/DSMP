#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>

#include <utility>      
#include <iostream>
#include <fstream>
#include <math.h>

#include "TFile.h"
#include "TSystem.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include <DSMP/DSMP_MTtoTT.h>

ClassImp(DSMP_MTtoTT)

DSMP_MTtoTT::DSMP_MTtoTT() {
    // options
    m_doJets = false;
    m_doFatJets = false;
    m_mc = false;
    m_applyFinalWeight = false;
    m_applyGRL = false;
    m_GRLs = "";
    m_doPRW = false;
    m_lumiCalcFiles = "";
    m_PRWFiles = "";
    m_PRWDefaultChannel = 0;

    // tools
    m_GRLTool = 0;
    m_PRWTool = 0;

    // output TinyTree
    m_outTree = 0;

    // output MetaData
    m_metaData = 0;

    // sum of weights
    m_sumOfWeights = 1.;

    // initalize vectors to 0
    initializeVectors();
}

EL::StatusCode DSMP_MTtoTT::initialize() {
    if ((!m_doJets && !m_doFatJets) || (m_doJets && m_doFatJets)) {
        std::cout << "Must select only one of m_doJets or m_doFatJets!" << std::endl;
        return EL::StatusCode::SUCCESS;
    }

    // get sum of weights
    if (m_mc && m_applyFinalWeight) setSumOfWeights();

    // GRL tool
    if (m_applyGRL) {
        m_GRLTool = new GoodRunsListSelectionTool("GoodRunsListSelectionTool");
        std::vector<std::string> grlVec;
        std::istringstream s(m_GRLs);
        std::string g;
        while (std::getline(s, g, ',')) grlVec.push_back(gSystem->ExpandPathName(g.c_str()));
        RETURN_CHECK("DSMP_MTtoTT::initialize()", m_GRLTool->setProperty("GoodRunsListVec", grlVec), "");
        RETURN_CHECK("DSMP_MTtoTT::initialize()", m_GRLTool->setProperty("PassThrough", false), "");
        RETURN_CHECK("DSMP_MTtoTT::initialize()", m_GRLTool->initialize(), "");
    }

    // PRW tool
    if (m_doPRW) {
        m_PRWTool = new CP::PileupReweightingTool("PileupReweightingTool");
        std::vector<std::string> lumiCalcVec;
        std::istringstream s1(m_lumiCalcFiles);
        std::string f1;
        while (std::getline(s1, f1, ',')) lumiCalcVec.push_back(f1);
        std::vector<std::string> prwVec;
        std::istringstream s2(m_PRWFiles);
        std::string f2;
        while (std::getline(s2, f2, ',')) prwVec.push_back(f1);
        RETURN_CHECK("dojetISR_MTtoTT::initialize()", m_PRWTool->setProperty("LumiCalcFiles", lumiCalcVec), "");
        RETURN_CHECK("dojetISR_MTtoTT::initialize()", m_PRWTool->setProperty("ConfigFiles", prwVec), "");
        if (m_PRWDefaultChannel) RETURN_CHECK("DSMP_MTtoTT::initialize()", m_PRWTool->setProperty("DefaultChannel", m_PRWDefaultChannel), "");
        RETURN_CHECK("dojetISR_MTtoTT::initialize()", m_PRWTool->setProperty("DataScaleFactor", 1.0/1.16), "");
        RETURN_CHECK("dojetISR_MTtoTT::initialize()", m_PRWTool->setProperty("DataScaleFactorUP", 1.0), "");
        RETURN_CHECK("dojetISR_MTtoTT::initialize()", m_PRWTool->setProperty("DataScaleFactorDOWN", 1.0/1.23), "");
        RETURN_CHECK("dojetISR_MTtoTT::initialize()", m_PRWTool->initialize(), "");
    }

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DSMP_MTtoTT::changeInput(bool firstFile) {
    if (firstFile) initializeOutTree();
   
    initializeInTree();
    copyMetaData();

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DSMP_MTtoTT::execute() {
    // get entry from tree
    wk()->tree()->GetEntry(wk()->treeEntry());

    // reset out tree branches
    resetBranches();
        
    out_runNumber = in_runNumber;
    out_eventNumber = in_eventNumber;

    // LASER - TODO: put in PRW
    
    // GRL
    if (!m_mc && m_applyGRL) {
        if (!m_GRLTool->passRunLB(in_runNumber, in_lumiblock)) {
            wk()->skipEvent();
            return EL::StatusCode::SUCCESS;
        } 
    }

    // trigger & categorization
    int runNumber = 0;
    if (!m_mc) runNumber = in_runNumber;
    else {} // LASER - TODO: handle random run number for MC
//    bool b_passJetTrigger = passJetTrigger(runNumber);
//    if (!b_passJetTrigger) return EL::StatusCode::SUCCESS;

    // fix weight
    float weight = in_weight;
    if (m_mc && m_applyFinalWeight) weight = weight / m_sumOfWeights;
    out_weight = weight;

    // get final variables
    TLorentzVector tlv1, tlv2;
    if (m_doJets && in_nj >= 2) {
        tlv1.SetPtEtaPhiE(in_ptj->at(0), in_etaj->at(0), in_phij->at(0), in_Ej->at(0));
        tlv2.SetPtEtaPhiE(in_ptj->at(1), in_etaj->at(1), in_phij->at(1), in_Ej->at(1));
    }
/* 
    if (m_doFatJets && in_nJ >= 2) {
        tlv1.SetPtEtaPhiM(in_ptJ->at(0), in_etaJ->at(0), in_phiJ->at(0), in_mJ->at(0));
        tlv2.SetPtEtaPhiM(in_ptJ->at(1), in_etaJ->at(1), in_phiJ->at(1), in_mJ->at(1));
    }
*/
    if (m_doFatJets && in_nJ >= 2) {
        tlv1.SetPtEtaPhiM(in_ptJ->at(0), in_etaJ->at(0), in_phiJ->at(0), in_mJ->at(0) / 1000.);
        tlv2.SetPtEtaPhiM(in_ptJ->at(1), in_etaJ->at(1), in_phiJ->at(1), in_mJ->at(1) / 1000.);
    }
    out_m1 = tlv1.M();
    out_pt1 = tlv1.Pt();
    out_m2 = tlv2.M();
    out_pt2 = tlv2.Pt();
    TLorentzVector tlvDSMP = tlv1 + tlv2;
    out_mDSMP = tlvDSMP.M();
    out_ptDSMP = tlvDSMP.Pt();

    // set category
    if (out_m1 < 50. && out_m2 < 50.) 
        out_category = _qq;
    if (out_m1 > 50. && out_m1 < 100. && out_m2 > 50. && out_m2 < 100.)
        out_category = _VV;
    if ((out_m1 > 50. && out_m1 < 100. && out_m2 > 100. && out_m2 < 140.) ||
        (out_m1 > 100. && out_m1 < 140. && out_m2 > 50. && out_m2 < 100.))
        out_category = _VH;
    if ((out_m1 > 50. && out_m1 < 100. && out_m2 > 140. && out_m2 < 200.) ||
        (out_m1 > 140. && out_m1 < 200. && out_m2 > 50. && out_m2 < 100.))
        out_category = _tV;
    if (out_m1 > 100. && out_m1 < 140. && out_m2 > 100. && out_m2 < 140.)
        out_category = _HH;
    if ((out_m1 > 100. && out_m1 < 140. && out_m2 > 140. && out_m2 < 200.) ||
        (out_m1 > 140. && out_m1 < 200. && out_m2 > 100. && out_m2 < 140.))
        out_category = _tH;
    if (out_m1 > 140. && out_m1 < 200. && out_m2 > 140. && out_m2 < 200.)
        out_category = _tt;
    if ((out_m1 < 50. && out_m2 > 50. && out_m2 < 100.) ||
        (out_m1 > 50. && out_m1 < 100. && out_m2 < 50.))
        out_category = _qV;
    if ((out_m1 < 50. && out_m2 > 100. && out_m2 < 140.) ||
        (out_m1 > 100. && out_m1 < 140. && out_m2 < 50.))
        out_category = _qH;
    if ((out_m1 < 50. && out_m2 > 140. && out_m2 < 200.) ||
        (out_m1 > 140. && out_m1 < 200. && out_m2 < 50.))
        out_category = _qt;
    if ((out_m1 > 200. && out_m2 > 50. && out_m2 < 100.) ||
        (out_m1 > 50. && out_m1 < 100. && out_m2 > 200.))
        out_category = _QV;
    if ((out_m1 > 200. && out_m2 > 100. && out_m2 < 140.) ||
        (out_m1 > 100. && out_m1 < 140. && out_m2 < 200.))
        out_category = _QH;
    if ((out_m1 > 200. && out_m2 > 140. && out_m2 < 200.) ||
        (out_m1 > 140. && out_m1 < 200. && out_m2 > 200.))
        out_category = _Qt;
    if (out_m1 > 200. && out_m2 > 200.)
        out_category = _QQ;
    
    m_outTree->Fill();

    return EL::StatusCode::SUCCESS;
}

void DSMP_MTtoTT::initializeVectors() {
    in_passedTriggers = 0;
    in_mJ = 0;
    in_ptJ = 0;
    in_etaJ = 0;
    in_phiJ = 0;
    in_Ej = 0;
    in_ptj = 0;
    in_etaj = 0;
    in_phij = 0;
}

void DSMP_MTtoTT::initializeInTree() {
    wk()->tree()->SetBranchAddress("runNumber", &in_runNumber);
    wk()->tree()->SetBranchAddress("eventNumber", &in_eventNumber);
    wk()->tree()->SetBranchAddress("lumiBlock", &in_lumiblock);
    wk()->tree()->SetBranchAddress("weight", &in_weight);
//    wk()->tree()->SetBranchAddress("passedTriggers", &in_passedTriggers);
    if (m_doJets) {
        wk()->tree()->SetBranchAddress("njets", &in_nj);
        wk()->tree()->SetBranchAddress("jet_E", &in_Ej);
        wk()->tree()->SetBranchAddress("jet_pt", &in_ptj);
        wk()->tree()->SetBranchAddress("jet_eta", &in_etaj);
        wk()->tree()->SetBranchAddress("jet_phi", &in_phij);
    }
    if (m_doFatJets) {
        wk()->tree()->SetBranchAddress("nfatjet", &in_nJ);
        wk()->tree()->SetBranchAddress("fatjet_tam_cal", &in_mJ);
        wk()->tree()->SetBranchAddress("fatjet_pt", &in_ptJ);
        wk()->tree()->SetBranchAddress("fatjet_eta", &in_etaJ);
        wk()->tree()->SetBranchAddress("fatjet_phi", &in_phiJ);
    }
}

void DSMP_MTtoTT::initializeOutTree() {
    m_outTree = new TTree("TinyTree", "TinyTree");
    
    m_outTree->Branch("runNumber", &out_runNumber, "runNumber/I");
    m_outTree->Branch("eventNumber", &out_eventNumber, "eventNumber/LI");
    m_outTree->Branch("category", &out_category, "category/I");
    m_outTree->Branch("weight", &out_weight, "weight/F");
    m_outTree->Branch("m1", &out_m1, "m1/F");
    m_outTree->Branch("pt1", &out_pt1, "pt1/F");
    m_outTree->Branch("m2", &out_m2, "m2/F");
    m_outTree->Branch("pt2", &out_pt2, "pt2/F");
    m_outTree->Branch("mDSMP", &out_mDSMP, "mDSMP/F");
    m_outTree->Branch("ptDSMP", &out_ptDSMP, "ptDSMP/F");

    wk()->addOutput(m_outTree);
}

void DSMP_MTtoTT::copyMetaData() {
    if (!m_metaData) {
        m_metaData = new TH1F("MetaData_TinyTree", "MetaData_TinyTree", 1, 0.5, 1.5);
        wk()->addOutput(m_metaData);
    }

    TH1F *md = (TH1F*) wk()->inputFile()->Get("MetaData");
    m_metaData->Fill(1, md->GetBinContent(3));
}

void DSMP_MTtoTT::setSumOfWeights() {
    TH1F *md = (TH1F*) wk()->inputFile()->Get("MetaData");
    m_sumOfWeights = md->GetBinContent(3);
}

void DSMP_MTtoTT::resetBranches() {
        out_runNumber = -999;
        out_eventNumber = -999;
        out_category = _none;
        out_weight = -999;
        out_m1 = -999;
        out_pt1 = -999;
        out_m2 = -999;
        out_pt2 = -999;
        out_mDSMP = -999;
        out_ptDSMP = -999;
}

bool DSMP_MTtoTT::passJetTrigger(int runNumber) {
    // 2015
    if (runNumber <= 284484) {
        for (auto t : *in_passedTriggers) {
            if (t == "HLT_j360" || t == "HLT_j360_a10r_L1J100" || t == "HLT_j360_a10_sub_L1J100" || t == "HLT_ht700_L1J75") return true;
        }
    }
    // 2016 period A
    else if (runNumber >= 296939 && runNumber <= 300287) {
        for (auto t : *in_passedTriggers) {
            if (t == "HLT_j340" || t == "HLT_j360_a10r_L1J100" || t == "HLT_j360_a10_lcw_L1J100" || t == "HLT_ht700_L1J75") return true;
        }
    }
    // 2016 period B-E
    else if (runNumber >= 300345 && runNumber <= 303892) {
        for (auto t : *in_passedTriggers) {
            if (t == "HLT_j380" || t == "HLT_j400_a10r_L1J100" || t == "HLT_j400_a10_lcw_L1J100" || t == "HLT_ht1000" || t == "HLT_ht1000_L1J100") return true;
        }
    }
    // 2016 period F-
    else if (runNumber >= 303943) {
        for (auto t : *in_passedTriggers) {
            if (t == "HLT_j380" || t == "HLT_j420_a10r_L1J100" || t == "HLT_j420_a10_lcw_L1J100" || t == "HLT_ht1000" || t == "HLT_ht1000_L1J100") return true;
        }
    }

    // didn't find a passed trigger
    return false;
}
