README file for CompareCases version 122
2011/06/24

Written by Steve Chall (stevec@renci.org), RENCI@Duke (www.renci.org)
in collaboration with and based on the work of:
*Joseph Y. Lo, Associate Research Professor of Medical Physics Grad Program, Radiology and Biomedical Engineering (joseph.lo@duke.edu)
*Shiva K. Das, Professor, Department of Radiation Oncology, Duke University School of Medicine (shiva.das@duke.edu)
*Vorakarn Chanyavanich, (vorakarn.chanyavanich@duke.edu)

Additional work by
*Bryan Harrawood (Brian.Harrawood@duke.edu)
*Carl Zhang

1. CompareCases Overview

CompareCases is an application that supports the exploration of several kinds of prostate cancer radiation therapy patient data, in particular examining similarities between a user-selected “query case” and one or several pre-existing “match cases.”  The goal is to maximize the similarity between query and match in order to use the match case as the best available starting point for treatment planning for the query case.

The open source CompareCases application is implemented in Microsoft Visual C++ 2010, using the Qt 4.71 libraries for graphical user interface development, VTK (The Visualization ToolKit) 5.6.1 for graphics and visualization, CMake for build management, and GitHub for source code control (https://github.com/steve9000gi/CompareCases).   

2. Graphical User Interface / Work Flow

2.1. Work Flow Overview

There are three primary dialogs that comprise the CompareCases work flow: 1) the Main Window for selecting a query case; 2) the Case Space Dialog for selecting a match case; and 3) the Compare Dialog, for examining similarities and differences between the query and match cases that have been selected.

2.2. The Main Window allows the user to select the top level directory or directories wherein reside the data used in the rest of the application (see section 3. below for details).  When one or more data directories have been selected, the user may choose from the corresponding dropdown lists to select a query case.  If a query case is selected, then the View Case Space button is enabled.  Clicking on this button opens the Case Space Dialog.

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

When the Compare and Case Space dialogs are open simultaneously, the user may change which match case is displayed by left-clicking on another patient point in the Case Space dialog.  Either the query or the match case displayed may be changed within the Compare dialog by editing the respective numeric spin boxes at the left edge of the Compare dialog.  Just above those spin boxes are buttons, currently hard-coded to display “Duke” only, intended in the future to provide the user with the capability of selecting cases from other institutions, e.g., Pocono and High Point, when such data are available.

View controls are coded to apply wherever possible to both query and match data display.

2.4.1. CT Data View Controls

The user may select an Axial, Sagittal, or Coronal view, any slice of the current view with the slider or associated spin box, or may animate through all slices in the current view by clicking on the “Auto-play” button.  If the query and match cases do not have the same number of slices, slice selection (and Auto-play) will display identically-numbered slices until the case with the fewest slices reached its final slice, after which it will remain at that final slice while the other case continues to its own last slice.

2.4.2. Projection Data View Controls

The “%Transparency” slider and associated spin box allows the user to select a value between 0 (the default, fully opaque) to 100%.

The “Gantry Angle” drop-down menu is currently hard-coded to allow selecting projection display from any of the angles 25, 75, 130, 180, 230, 280, or 335 degrees. 

The view position may be arbitrarily reset independently for each projection display by click-dragging within the display of interest.  Dragging rotates the view; shift-dragging translates in x and y; control-shift-dragging translates in z.  Selecting an angle from the “Gantry Angle” drop-down menu restores the view to the selected angle.

The “Flat-shaded” check box toggles projection surface shading characteristics.

The “Axes” checkbox toggles the display of the standard xyz axes at the origin.

2.4.3. “View Femoral Heads” 

The “View Femoral Heads” checkbox affects both Projection and DVH displays: it toggles visibility of the left and right femoral heads in the Projection display, and additionally the corresponding data curves in the DVH graphs. 

2.4.4. Overlay

The “Overlay” checkbox toggles whether or not to superimpose the match DVH plots as dashed lines on top of the query DVH display.

3. Input Data Organization and Formats

3.1. Required Directory Organization and Naming

In order for the Compare Cases application to locate patient data, the subdirectories beneath the top-level directory (selected with the appropriate buttons under “Load Institutions:” in the Main Window dialog) must be organized as follows:

Subdirectory “CT” contains a separate subdirectory for each case, named “Patient<XXX>” where <XXX> is a three-digit integer (left-padded with zeros where necessary, 000-999, e.g., “Patient007”).  Inside each “Patient<XXX>” directory must be a directory named “Primary” that contains only CT file sequences whose names are of the format “CT*.dcm” where “*” is a sequence of digits and ‘.’ characters.  There may be and currently are other directories and files in each “Patient<XXX>” directory, but they cannot be in the Primary directory for the CompareCases application to work correctly.

Subdirectory “DVHdata” contains for each patient one text file whose name is of the format DVH<XXX>.txt where <XXX> is a three-digit integer (front-padded with zeros where necessary, 000-999, e.g., “DVH007.txt”).

Subdirectory “overlap” contains a text file named Duke_struct_overlap<XXX>.txt for each of the gantry angles 025, 075, 130, 180, 230, 280, and 330, where <XXX> is the corresponding (possibly front-padded with zeroes) three-digit gantry angle.   Each file contains positive integer values for its gantry angle for all available cases between 3-148 inclusive.

Subdirectory “structures” contains subdirectories named <XXX> where <XXX> is a three-digit (possibly zero left-padded) case number.  Inside each sub-subdirectory are vertices and faces files named according to the following conventions for naming projection data files:
       <inFileType>_<patient number>_<structure>.out
where 
	<inFileType> = [ "faces" | "vertices" ],
       <patient number> is a 3-digit integer (possibly front-padded with 0's),
and
       <structure> = [ "bladder" | "LtFem" | "PTV" | "rectum" | "RtFem" ].

3.2. Overlap Data Format

Overlap data is accessed via text files.  The first row is comprised of the following column headers:

Case number
PTV size
rec size
bla size
PTV+rec
PTV+bla
PTV+rec+bla

Each subsequent row represents those values for one case. 

3.3. CT Data Format

CT data are in standard DICOM format. 

3.4. Projection Data Format

Each line of a vertices file is comprised of three space-separated decimal numbers representing the x, y, and z values for that vertex.

Each line of a faces file is comprised of three space-separated integers representing which vertices from the corresponding vertices file make up a given triangular face.

3.4.1. Projection Data Overview

3.4.2. Creating Projection Data

A modified visualStruct3D.m – based on a CERR function -- exports vertices and faces data from MATLAB-based CERR in the format required for reading by the CompareCases application.
From http://cerr.info/cerrwiki/index.php/Main_Page:

CERR (pronounced 'sir'), stands for Computational Environment for Radiotherapy Research. CERR is a software platform for developing and sharing research results using radiation therapy treatment planning information. CERR is written in the widely-used Matlab language (version 6.1 or later), allowing for low-cost development of visualization and analysis tools. CERR is actively supported by the Division of Bioinformatics and Outcomes Research, Dept. of Radiation Oncology, Washington University School of Medicine, and has a wide variety of dose, imaging, and structure analysis and comparison tools. CERR is free to use for research projects. Clinical use on individual patient cases is prohibited. 

3.4. Dose Volume Histogram Data

4. Bugs

Query projection view shifts upward the first time it’s user-modified.  

If “Overlay” is on, changing the match case may not update the overlay on the query DVH display, even though the title may update.

If the user selects any directory for Pocono or High Point in the Main Window, then selects one of the dummy case numbers that are made available for that non-Duke institution, and then clicks on the “View Case Space” button, an exception is thrown and the program crashes.

If the user changes the view by dragging within a projection display and then subsequently selects an angle from the “Gantry angle” dropdown menu, some view parameters (e.g., distance from projection centroid) may not be restored to their default values.

Quitting the application may involve manually closing multiple dialogs.  Furthermore, the various “OK” and “Cancel” buttons are undifferentiated with respect to program behavior: “OK” saves nothing, “Cancel” cancels nothing.

A great deal of the data is still missing for many cases.  In particular, DVH data exists for only about 9 cases.  Therefore, if a match case is selected with missing data from the Case Space dialog, a “missing data” error message may appear when the Compare Cases button is clicked, or (if it’s DVH data that’s missing) the Compare dialog may open with a blank Match DVH display.  If structure data (i.e., one or more vertices or faces files), the Compare dialog may simply not open.

During CT Display “Auto-play” (Compare dialog), neither the Slice spin box nor slider update values until the Auto-play animation is complete.

Disfiguring display artifacts appear in the Projection displays for Transparency values > 0% (Compare dialog) due to errors in triangle depth sorting.  This is especially noticeable when “Flat shaded” is unchecked.

In the Compare Cases dialog, the “Selected case” icon always appears at the origin, wherever the selected case is actually located.

5. Future Work

Implement “crosshairs” or “white line” mechanism to link geometric positions between projections and CT displays

Ensure that query and match CT displays are aligned with respect to PTV centers.

Add 3rd dimension to Case Space dialog by adding slider[s] for selecting range of (average) MI values applied to selecting appropriate subset of patient data candidate points.

Now have MI data for 100 Duke cases which need to be integrated into the application.

Incorporate patient data from Pocono, High Point, and possibly other institutions.

Add PTV overlay to CT displays.

Add dynamic recomputation of MI values when a new patient’s data is added to the database.

Add dynamic generation of projections to support non-Duke gantry angles.

Explore possible approaches to a higher dimensional (3+) display (for simultaneously evaluating a larger number of match case selection criteria) for Case Space Dialog.

Apply size or color as depth cues to indicate position in z for points on Case Space dialog. E.g., the redder and/or larger a point, the closer it is, the bluer and/or smaller it is, the more distant.

Lock camera positions between match and query cases so that interactive camera manipulation in either also applies to the other.

Implement history of most recent (say, 5?) match cases, perhaps with a dropdown list, so that user may easily recall what previous match candidates s/he has visited and can return to any previous by simply selecting item.

Support overlay of multiple match candidates’ data for query DVH diagram.

6. Additional reading

Vorakarn Chanyavanich, Shiva K. Das, William R. Lee, Joseph Y. Lo, Knowledge-based IMRT treatment planning for prostate cancer, Med. Phys. 38, 2515 (2011), http://online.medphys.org/resource/1/mphya6/v38/i5/p2515_s1

