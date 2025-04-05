# This script can be used to test the effects of different environmental
#  options within SAC. Copy it to the directory from which you want to
#  launch SAC and edit it to set the various options. If the name of your
#  copy is mysacinit.csh, the settings will be set for the current session
#  by entering
#
#  . ./mysacinit.csh
#
#  and then run SAC.  Once one is satisfied, one should put the 
#  settings directly in your start-up script.
#
# Initialize the SAC Environment (only SACHOME may need to be changed) 
export SACHOME=/Users/edahi/Desktop/moment_inversion/sac
export PATH=${PATH}:${SACHOME}/bin
export SACAUX=${SACHOME}/aux

# Uncomment the desired options below to change from the default and
# comment out the default option
#
# SAC_DISPLAY_COPYRIGHT 
# Undefined or 1 -- Show Copyright Notice on Initialization ( Default )
#              0 -- Do not show Copyright Notice
# The copyright is only displayed during the initialization of SAC
# It contains the build date, version number and copyright information
# Default: SAC_DISPLAY_COPYRIGHT=1
export SAC_DISPLAY_COPYRIGHT=1
# export SAC_DISPLAY_COPYRIGHT=0

# SAC_USE_DATABASE
# Undefined or 1 --        Use SeisMgr Database ( Default )
#              0 -- Do Not Use SeisMgr Database
# The SeisMgr database attempts to keep the CSS data fields in line
#  with those in the SAC header.  If you are handling CSS data it 
#  would be wise to keep the database on.  Using the SeisMgr database
#  currently can be very slow due when handling hundreds of files
#  and turning it off should show a dramatic speed increase.
#  HOWEVER, some features of SAC still depend on its existence, which is
#  why the default is to have it on.
# Default: SAC_USE_DATABASE=1
export SAC_USE_DATABASE=1
# export SAC_USE_DATABASE=0

# SAC_PPK_LARGE_CROSSHAIRS
# Undefined or 0 - Tiny Cross Hairs    ( Default )
#              1 - Cross Hairs of the Full Plot Window
# In older version of SAC, ppk displayed crosshairs on the window
# which extended the entire window length.  To some, this was desirable.
# To utilize this behavior again, set SAC_PPK_LARGE_CROSSHAIRS to 1
# Default: SAC_PPK_LARGE_CROSSHAIRS=0
export SAC_PPK_LARGE_CROSSHAIRS=0
# export SAC_PPK_LARGE_CROSSHAIRS=1

