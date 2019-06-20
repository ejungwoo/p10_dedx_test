#./example myrun

#root -l -q -b make_cluster.C\(0\);
#root -l -q -b make_cluster.C\(2\);
#root -l -q -b make_cluster.C\(4\);

root -l -q -b   make_track.C\(0,20\);
root -l -q -b   make_track.C\(0,50\);
root -l -q -b   make_track.C\(0,100\);
root -l -q -b   make_track.C\(0,200\);

root -l -q -b   make_track.C\(2,20\);
root -l -q -b   make_track.C\(2,50\);
root -l -q -b   make_track.C\(2,100\);
root -l -q -b   make_track.C\(2,200\);

root -l -q -b   make_track.C\(4,20\);
root -l -q -b   make_track.C\(4,50\);
root -l -q -b   make_track.C\(4,100\);
root -l -q -b   make_track.C\(4,200\);
