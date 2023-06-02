
# remove //TODOUNCOMMENT from yaml file
sed -e "s/\/\/TODOUNCOMMENT//g" -i .backup1 coffee.yaml

# remove complete line if TODOremove is in it
sed -i.backup2 '/TODOremove/d' coffee.yaml