#include <EventLoop/Job.h>
#include <EventLoop/StatusCode.h>
#include <EventLoop/Worker.h>
#include <EventLoop/OutputStream.h>

#include <SampleHandler/MetaFields.h>

#include "xAODRootAccess/Init.h"
#include "xAODRootAccess/TEvent.h"
#include "xAODRootAccess/TStore.h"

#include "xAODEventInfo/EventInfo.h"
#include "xAODJet/JetContainer.h"

#include "TH1.h"

#include <DSMP/DSMP_DAODtoMT.h>

ClassImp(DSMP_DAODtoMT)

DSMP_DAODtoMT::DSMP_DAODtoMT() {
    m_saveJets = true;
    m_saveFatJets = true;
    m_jetContainerName = "";
    m_fatJetContainerName = "";
    m_eventInfoDetailStr = "";
    m_jetDetailStr = "";
    m_fatJetDetailStr = "";
}

EL::StatusCode DSMP_DAODtoMT::setupJob(EL::Job& job) {
    job.useXAOD();
    xAOD::Init("DSMP_DAODtoMT").ignore();

    EL::OutputStream outTree("MiniTree");
    job.outputAdd(outTree);

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DSMP_DAODtoMT::histInitialize () {
  RETURN_CHECK("DSMP_DAODtoMT::histInitialize()", xAH::Algorithm::algInitialize(), "");
  
  return EL::StatusCode::SUCCESS;
}

EL::StatusCode DSMP_DAODtoMT::initialize() {
    if ((!m_saveJets && !m_saveFatJets) || (m_saveJets && m_saveFatJets)) {
        std::cout << "Must require only one of m_saveJets or m_saveFatJets!" << std::endl;
        return EL::StatusCode::FAILURE;
    }

    m_event = wk()->xaodEvent();
    m_store = wk()->xaodStore();

    TTree *outTree = new TTree("tree", "tree");
    TFile *treeFile = wk()->getOutputFile("MiniTree");
    outTree->SetDirectory(treeFile);
    
    m_tree = new DSMP_MiniTree(m_event, outTree, treeFile);
    m_tree->AddEvent(m_eventInfoDetailStr);
    if (m_saveJets) m_tree->AddJets(m_jetDetailStr);
    if (m_saveFatJets) m_tree->AddFatJets(m_fatJetDetailStr);

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DSMP_DAODtoMT::execute() {
    // get event info
    const xAOD::EventInfo *eventInfo = 0;
    RETURN_CHECK("DSMP_DAODtoMT::execute()", HelperFunctions::retrieve(eventInfo, "EventInfo", m_event, m_store), "");

    // event weight
    if (eventInfo->eventType(xAOD::EventInfo::IS_SIMULATION)) {
        float mcEventWeight = eventInfo->mcEventWeight();
        double xs = wk()->metaData()->castDouble(SH::MetaFields::crossSection, 1);
        double eff = wk()->metaData()->castDouble(SH::MetaFields::filterEfficiency, 1);
        double kfactor = wk()->metaData()->castDouble(SH::MetaFields::kfactor, 1);
        eventInfo->auxdecor<float>("weight") = mcEventWeight * xs * eff * kfactor;
    }
    else {
        eventInfo->auxdecor<float>("weight") = 1.;
    }

    // get jets
    const xAOD::JetContainer *jets = 0;
    if (m_saveJets) RETURN_CHECK("DSMP_DAODtoMT::execute()", HelperFunctions::retrieve(jets, m_jetContainerName, m_event, m_store), "");
    
    // get fat jets
    const xAOD::JetContainer *fatJets = 0;
    if (m_saveFatJets) RETURN_CHECK("DSMP_DAODtoMT::execute()", HelperFunctions::retrieve(fatJets, m_fatJetContainerName, m_event, m_store), "");

    // fill tree branches
    m_tree->FillEvent(eventInfo, m_event);
    if (m_saveJets) m_tree->FillJets(jets);
    if (m_saveFatJets) m_tree->FillFatJets(fatJets);
    m_tree->Fill();

    return EL::StatusCode::SUCCESS;
}

EL::StatusCode DSMP_DAODtoMT::histFinalize() {
    RETURN_CHECK("DSMP_DAODtoMT::histFinalize()", xAH::Algorithm::algFinalize(), "");

    // copy metadata to output minitree
    TFile *fileMD = wk()->getOutputFile("metadata");
    TH1D *histEventCount = (TH1D*) fileMD->Get("MetaData_EventCount");
    TFile *treeFile = wk()->getOutputFile("MiniTree");
    TH1F *thisHistEventCount = (TH1F*) histEventCount->Clone("MetaData");
    thisHistEventCount->SetDirectory(treeFile);

    return EL::StatusCode::SUCCESS;
}
