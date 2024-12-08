#!/bin/bash
echo -n "Dossier pour enregistrer le hash : "
read dossiers
mkdir backup_data/$dossiers
cp network_data.bin backup_data/$dossiers/network_data.bin
cp examples_data.bin backup_data/$dossiers/examples_data.bin
sha256sum network_data.bin > backup_data/$dossiers/network_data.bin.sha256
sha256sum examples_data.bin > backup_data/$dossiers/examples_data.bin.sha256
