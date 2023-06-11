# CKGBA
Commander Keen Galaxy Engine for the GBA

# Notes
This project uses some code from: http://k1n9duk3.shikadi.net/files/modding/keensource456.zip
More info can be found: https://pckf.com/viewtopic.php?t=11505

# Adding / Modifying Actors
All cases where a new object is created via GetNewObj(), 
if the object name 'new' it must be changed to 'ck_newobj'

Also, the function NewState() now takes an extra parameter -
The type of sprite denoted with 'CKS_'

# Porting / Modding
A readme is available for the process of modding this source port
