#include <TChain.h>
#include <iostream>
#include <TBranch.h>
#include <TH1D.h>
#include <TLorentzVector.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TROOT.h>
#include <TString.h>

// Location of the root file to be analyzed
const TString inputFile = "root://xrootd-local.unl.edu///store/user/wtabb/DrellYan_13TeV_2016/v2p6/DYLL_M50toInf/base/ntuple_skim_sig_990.root";

// Name of the tree in the root file
const TString treeName = "recoTree/DYTree";

void Analyze(Long64_t nEntries = -1000)
{
	// Keeps from trying to open a canvas for plots
	gROOT->SetBatch(true);

	// Define chain to hold trees
	TChain*chain = new TChain(treeName);	
	
	// Add file to chain
	chain->Add(inputFile);

	// Get number of entries in chain
	// This is set so that a negative number can be input to loop over all events
	if(nEntries < 0) nEntries = chain->GetEntries();
	cout << "Entries loaded: " << nEntries << endl;

	// Initialize branch variables
	int MPSIZE = 500;
	int Nelectrons;
	double Electron_pT[MPSIZE];
	double Electron_eta[MPSIZE];
	double Electron_phi[MPSIZE];
	double mass = 0.000511; // electron mass

	// Intialize Branches
	TBranch*b_Nelectrons;
	TBranch*b_Electron_pT;
	TBranch*b_Electron_eta;
        TBranch*b_Electron_phi;

	// Set branch address for chain
	chain->SetBranchAddress("Nelectrons",&Nelectrons,&b_Nelectrons);
	chain->SetBranchAddress("Electron_pT",&Electron_pT,&b_Electron_pT);
	chain->SetBranchAddress("Electron_eta",&Electron_eta,&b_Electron_eta);
	chain->SetBranchAddress("Electron_phi",&Electron_phi,&b_Electron_phi);

	// Initialize histogram
	int nBins = 100;
	double lowEdge = 60.0;
	double highEdge = 120;
	TString histName = "histInvariantMass_";
	histName += nEntries;
	histName += "Entries";
	TH1D*histInvariantMass = new TH1D(histName,"",nBins,lowEdge,highEdge);

	// Loop over events
	for(Long64_t iEvent=0;iEvent<nEntries;iEvent++){
		// Initialize pT variables
		double ptLead = -1000.0;
		double ptSub  = -1000.0;

		// Initialize indices of electrons
		int idxLead = -1;
		int idxSub  = -1;

		// Get entries from trees in chain
		chain->GetEntry(iEvent);

		// Discard events with fewer than two electrons
		if(Nelectrons<2) continue;

		// loop over electrons in each event
		// and select the two with the highest pT
		for(int iEle=0;iEle<Nelectrons;iEle++){
			if(Electron_pT[iEle] > ptLead){
				// electron with highest pT
				ptLead = Electron_pT[iEle];
				idxLead = iEle;	
			}
			if(Electron_pT[iEle] > ptSub && Electron_pT[iEle] < ptLead){
				// electron with second highest pT
				ptSub = Electron_pT[iEle];
				idxSub = iEle;
			}
		}// end iEle loop

		// Make sure top two pT electrons are correctly chosen
		if(ptSub>ptLead){
			cout << "****************************" << endl;
			cout << "ERROR: ptSub > ptLead" << endl;
			cout << "Event: " << iEvent << endl;
			cout << "ptSub = " << ptSub << endl;
			cout << "ptLead = " << ptLead << endl;
			cout << "****************************" << endl;
		}// end if ptSub > pt lead
				
		// Initialize TLorentzVector objects to calculate invariant mass
		TLorentzVector vLead;
		TLorentzVector vSub;

		// get variables for lead and sub electrons
		double etaLead = Electron_eta[idxLead];
		double etaSub  = Electron_eta[idxSub];
		double phiLead = Electron_phi[idxLead];
		double phiSub  = Electron_phi[idxSub];

		// Define TLorentzVector objects
		vLead.SetPtEtaPhiM(ptLead,etaLead,phiLead,mass);
		vSub.SetPtEtaPhiM(ptSub,etaSub,phiSub,mass);

		// Calculate invariant mass of the two selected electrons
		double invMass = (vLead+vSub).M();
		histInvariantMass->Fill(invMass);
	}// end event loop

	// Create base save name for root files and plots
	TString saveBase = "output_data/invariantMass_";
	saveBase += nEntries;
	saveBase += "Entries";

	// Create canvas to draw plot on
	TCanvas*c1 = new TCanvas("c1","",0,0,1000,1000);
	c1->SetGrid();
	histInvariantMass->GetXaxis()->SetTitle("dielectron invariant mass [GeV]");
	histInvariantMass->SetFillColor(kOrange-2);
	histInvariantMass->Draw("hist");

	// Create save name for the plot
	TString canvasSave = saveBase;
	canvasSave += ".png";
	
	// Save plot
	c1->SaveAs(canvasSave);

	// Create save name for root file
	TString rootSave = saveBase;
	rootSave += ".root";

	// Create root file and save histogram to it
	TFile*saveFile = new TFile(rootSave,"recreate");
	histInvariantMass->Write();
	saveFile->Close();	
}// end function

