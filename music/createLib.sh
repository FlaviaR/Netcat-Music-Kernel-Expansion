#!/bin/sh

ogg2Array "$1"

ogg=ogg
name="$1"

noExt=${name%ogg}
noExt=${noExt::-1}
track=trk"$2".c

trackAdd="$2"
sub=1
lastTrack=$(( ${trackAdd} - ${sub} ))
lastTrack2=$(( ${lastTrack} - ${sub} ))

echo "#include" '"'../netcat.h'"' >> ${track}
echo "#define NETCAT_CPI_TRK"$2"_LEN INSERT_SIZE_HERE"  >> ${track}
echo struct netcat_track netcat_cpi_trk"$2" = {  >> ${track}
echo	.name = '"'${noExt}'"',  >> ${track}
echo	.len  = NETCAT_CPI_TRK"$2"_LEN,  >> ${track}
echo	.data = {  >> ${track}
echo "#include" '"'${noExt}.h'"'  >> ${track}
echo	}  >> ${track}
echo "};"  >> ${track}
mv ${track} ../tracks

sed -i '$s/.$//' ${noExt}.h
mv ${noExt}.h ../tracks

sed -i '/extern struct netcat_track netcat_cpi_trk'${lastTrack}';/a extern struct netcat_track netcat_cpi_trk'${trackAdd}';' ../netcat_main.c
sed -i '/&netcat_cpi_trk'${lastTrack}'};/d' ../netcat_main.c
sed -i '/&netcat_cpi_trk'${lastTrack2}',/a \\t\t\t\t\t&netcat_cpi_trk'${lastTrack}',\n\t\t\t\t\t&netcat_cpi_trk'${trackAdd}'};' ../netcat_main.c

sed -i '/trk'${lastTrack}'.o/a netcat-y+= tracks/trk'${trackAdd}'.o' ../Makefile

echo "\nFinished updating the Makefile, netcat_main.c, creating the trk.c file, and updating the track's library\n."

gedit ../tracks/${track}

