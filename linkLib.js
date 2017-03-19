#!/usr/bin/env node

const env = {
  libDir: 'assets/extern_lib/',
  localIncludeDir: 'includes/',
  configFileName: 'lib_config.json',
};

// let finalError = []; we should recap at the end if there is some errors

const fs = require('fs');

const copyFile = (source, target) => {
  fs.createReadStream(source).pipe(fs.createWriteStream(target));
}

const buildLib = (file) => {
  try {
    fs.accessSync(env.libDir + file + '/' + env.configFileName);
  } catch (err) {
    return;
  }
  const lib = JSON.parse(fs.readFileSync(env.libDir + file + '/' + env.configFileName));

  console.log(`Building ${lib.name} library:\n`);

  if (lib.config.include_name) {
    console.log('Copy of includes files:');
    lib.config.include_name.forEach((includeFile) => {
      console.log(`${includeFile}`);
      copyFile(env.libDir + file + '/' + includeFile, env.localIncludeDir + includeFile);
    });
  }

};

const getLib = (err, files) => {
  if (err) {
    throw err;
  }
  files.forEach((file) => {
    const fileStat = fs.statSync(env.libDir + file);
    if (fileStat.isDirectory()) {
      buildLib(file);
    }
  });
};

// main call
fs.readdir(env.libDir, getLib);
