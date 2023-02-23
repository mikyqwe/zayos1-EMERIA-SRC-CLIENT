###########################
### YMIR WON'T SAVE THE FOLLOWING FIELDS, SO YOU WILL GET 0 FROM THEM WHEN YOU UNPACK THE MOB_PROTO: ###
gold min, gold max, polymorph item, mount capacity
skill_*
sp_*
NB: damage multiply is truncated, so you will get 1 instead of 1.4 and so on
NB2: due to a bug, all the mob_protos packed with an unfixed/default DumpProto tool will get the race flag set to 0 if it contains more than 1 value (the bug is about the splitting feature)
###########################
### YMIR WON'T SAVE THE FOLLOWING FIELDS, SO YOU WILL GET 0 FROM THEM WHEN YOU UNPACK THE ITEM_PROTO: ###
addon_type
###########################
### USAGE ###
-i process item proto
-m process mob proto

-d print debug errors in log.txt

-p pack phase
-u unpack phase

> DumpProto.exe -pi
process item_proto.txt to item_proto

> DumpProto.exe -pm
process mob_proto.txt to mob_proto

> DumpProto.exe
> DumpProto.exe -pmi
process item/mob_proto.txt to item/mob_proto

> DumpProto.exe -dpmi
process item/mob_proto.txt to item/mob_proto printing debug errors

> DumpProto.exe -um
process mob_proto to mob_proto.txt

> DumpProto.exe -ui
process item_proto to item_proto.txt

> DumpProto.exe -umi
process item/mob_proto to item/mob_proto.txt

> DumpProto.exe -dumi
process item/mob_proto to item/mob_proto.txt printing debug errors
