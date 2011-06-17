README file for CompareCases 2011/06/17

Written by Steve Chall (stevec@renci.org), RENCI@Duke (www.renci.org)

in collaboration with and based on the work of:
*Joseph Y. Lo, Associate Research Professor of Medical Physics Grad Program, Radiology and Biomedical Engineering (joseph.lo@duke.edu)
*Shiva K. Das, Professor, Department of Radiation Oncology, Duke University School of Medicine (shiva.das@duke.edu)
*Vorakarn Chanyavanich, (vorakarn.chanyavanich@duke.edu)

Additional work by
*Bryan
*Carl Zhang

1. CompareCases Overview

CompareCases is an application that supports the exploration of several kinds of prostate cancer radiation therapy patient data, in particular examining similarities between a user-selected “query case” and one or several pre-existing “match cases.”  The goal is to maximize the similarity between query and match in order to use the match case as the best available starting point for treatment planning for the query case.  

2. Graphical User Interface / Work Flow

2.1. Work Flow Overview

There are three primary dialogs that comprise the CompareCases work flow: 1) the Main Window for selecting a query case; 2) the Case Space Dialog for selecting a match case; and 3) the Compare Dialog, for examining similarities and differences between the query and match cases that have been selected.

2.2 The Main Window allows the user to select the top level directory or directories wherein reside the data used in the rest of the application (see section 3. below for details).  When one or more data directories have been selected, the user may choose from the corresponding dropdown lists to select a query case.  If a query case is selected, then the View Case Space button is enabled.  Clicking on this button opens the Case Space Dialog.

Selecting input data paths

When CompareCases starts, one of two things occurs:
1) If the user has previously indicated to the application where to find the top-level directory for patient data, then there exists (in the . directory for the executable) a text file named PathToDukeData which contains as a character string the full pathname  for that top-level directory.  CompareCases then uses that pathname to look at the Duke data and to populate the Select Query Case popup lists.

2) If this is the first time this installation of CompareCases is run, the first thing the user will see is the “Select directory for Duke case data” dialog.  Completion of this selection process also creates the ./PathToDukeData file in which the selected data path is saved. Subsequent executions of the CompareCases application will look here, thus obviating the necessity of repeating the directory selection process.  

The user may at any time select another location for a top—level case data directory by clicking on the appropriately labeled “Load Institutions” button, currently “Duke,” “Pocono”, or “High Point.”

Note:  Currently only Duke data are available.  Any functionality involving Pocono or High Point at this time displays only dummy data for the purpose of putting in hooks for future development and for demonstrating aspects of projected future work flow. 

2.3. Case Space Dialog

The Case Space Dialog is opened from the Main Window, after a query case has been selected, by clicking on the Main Window’s “View Case Space” button.  

In the upper left hand corner the selected Query case is shown by institution name (e.g., “Duke”) and patient number.  Directly beneath is shown the corresponding information for the Match case.  When the Case Space dialog is initially opened the Match case is shown as “<not selected>.”  

The dialog shows a Cartesian graph at the origin of which is placed the Query case – a different Query case will have a different origin with respect to the location of the potential Match cases.  

At this writing (2011/06/17) the horizontal axis is hard-coded to represent the degree of PTV and bladder overlap at a 180 degree gantry angle:  the horizontal distance of any patient’s point on the graph from the origin (i.e., from the Query case value) is directly proportional to the similarity of that patient’s recorded PTV + bladder overlap.

Similarly, the vertical axis represents the amount of PTV + rectum overlap from a 180 degree gantry angle: the greater the distance a patient point is from the Query case, the more dissimilar it is to the Query case with respect to PTV + rectum overlap.

The general intent is to allow the user to select a Match patient that is as similar as possible to the Query case in terms of the displayed parameters.  This selection process has not been automated (say, according to minimum distance from the Query case) because there are many possible criteria that may be used to determine similarity -- the two used here (PTV + bladder overlap, PTV + rectum overlap, both at a 180 degree angle) represent only a simple prototype for a selection process that needs to be expanded to provide a much broader availability of parameters.

A match case is selected by left-clicking on one of the patient points.  Match case selection is indicated by the updating of the “Match Case” information in the upper left corner of the Case Space Dialog.  Hovering over a patient point displays, for that patient, the institution, the patient number, and the values used for horizontal (PTV + bladder overlap) and vertical (PTV + rectum overlap) positioning on the graph, respectively.  The match case may be changed by simply left-clicking on another patient point.

When a match case has been selected the “Compare Cases” button in the lower-left corner of the Case Space dialog is enabled.  Left-clicking on the “Compare Cases” button opens the Compare dialog.

2.4. Compare Dialog

The Compare dialog displays three kinds of visual data for both the selected query (top) and match (bottom) cases: left to right, 1) CT images, 2) structure projections from several BEVs (Beam’s Eye Views, i.e., gantry angles), and 3) Dose Volume Histograms (DHV).

When the Compare and Case Space dialogs are open simultaneously, the user may change which match case is displayed by left-clicking on another patient point in the Case Space dialog.  Either the query or the match case displayed may be changed within the Compare dialog by editing the respective numeric spin boxes at the left edge of the Compare dialog.  Just above those spin boxes are buttons, currently hard-coded display Duke, intended in the future to provide the user with the capability of selecting cases from other institutions, e.g., Pocono and High Point, when such data are available.

3. Input Data Organization and Formats

3.1. Required Directory Organization and Naming

In order for the Compare Cases application to locate patient data, the subdirectories beneath the top-level directory (selected with the appropriate buttons under “Load Institutions:” in the Main Window dialog) must be organized as follows:

Subdirectory “CT” contains a separate subdirectory for each case, named “PatientXXX” where “XXX” is a three-digit integer (left-padded with zeros where necessary, 000-999, e.g., “Patient007”).  Inside each “PatientXXX” directory must be a directory named “Primary” that contains only CT file sequences whose names are of the format “CT*.dcm” where “*” is a sequence of digits and ‘.’ characters.  There may be and currently are other directories and files in each “PatientXXX” directory, but they cannot be in the Primary directory for the CompareCases application to work correctly.

Subdirectory “DVHdata” contains for each patient one text file whose name is of the format DVHXXX.txt where “XXX” is a three-digit integer (left-padded with zeros where necessary, 000-999, e.g., “DVH007.txt”).  

3.2 Overlap Data

3.3 CT Data

3.4 Projection Data

3.4.1 Projection Data Overview

3.4.2 Creating Projection Data

CERR / MATLAB

The Carl Zhang/Vorakarn Chanyavanich convention for naming projection data files:
       <inFileType>_<patient number>_<structure>.out
where <inFileType> = [ "faces" | "vertices" ],
       <patient number> is a 3-digit integer (possibly front-padded with 0's),
and
       <structure>  = [ "bladder" | "LtFem" | "PTV" | "rectum" | "RtFem" ].


3.4 Dose Volume Histogram Data

Overlay

4. Bugs

5. Future Work

6. Additional reading

