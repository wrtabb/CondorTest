# CondorTest
This package is intended to test the various methods needed for my broader analysis package. At present, my analysis package works on t3.unl.edu but does not work on crane.unl.edu. The main points that need to be addressed in this simplifed package are:
- Successfully submit a job to HTCondor  
- Use a proxy to access data stored on Hadoop using XROOTD  
- Use Docker so that the machine where the job is actually run has the needed dependencies  

After cloning this repo, make sure to create a directory called 'logs' to save all Condor logs.  
To submit the Condor jobs, simply type:  
```
./SubmitJob.sh
```
The way this is set up to work:  
- Load a root file located here: /mnt/hadoop/user/uscms01/pnfs/unl.edu/data4/cms/store/user/wtabb/DrellYan_13TeV_2016/v2p6/DYLL_M50toInf/base/ntuple_skim_sig_990.root  
- Loop over number of events, which are given to the main function by the submission scripts, and produce a plot of invariant mass  
- Save plot in a root file  
- Save canvas as a .png file (all files will save in a directory called 'output_files')   

The script, SubmitJob.sh, sends the job to Condor with a few different numbers of events. If the number of events is set at -1000, the function will loop over all events in the file.

Here is an explanation of each script in this repo:  
- **SubmitJob.sh**: The submission script. This simply submits the job to Condor once for each of a list of variables defined in the script.  
- **condor_control.condor**: This is the Condor submission script.   
- **ProcessScript.sh**: Sets up the CMSSW environment needed and then runs the python script with the argument given to it by Condor submission script.  
- **rootlogon.C**: When ROOT is executed, this file loads and compiles the ROOT script, Analyze.C.  
- **ProcessScript.py**: A wrapper for the ROOT script. It loads the function from Analyze.C and gives it the argument fed to it from ProcessScript.sh.  
- **Analyze.C**: The ROOT script which contains the main function that does the analysis.  
