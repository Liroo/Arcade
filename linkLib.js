#!/usr/bin/env node

const env = {
  libDir: 'assets/extern_lib/',
  opaqueDir: 'assets/shared_lib/opaque/',
  sharedDir: 'assets/shared_lib/shared/',
  configFileName: 'lib_config.json',
};

// let finalError = []; we should recap at the end if there is some errors
var directoryInUse;
const fs = require('fs');
const childProcess = require('child_process');

const buildLib = (file) => {
  try {
    fs.accessSync(directoryInUse + file + '/' + env.configFileName);
  } catch (err) {
    return;
  }
  const lib = JSON.parse(fs.readFileSync(directoryInUse + file + '/' + env.configFileName));

  console.log(`Building ${lib.name} library:`);

  switch (lib.type) {
    case 'include':
      console.log('library type: include');
      break;
    case 'library':
      console.log('library type: library');
      break;
    case 'test':
      if (process.argv[3] != 'test') {
        console.log('to install test library, run `linkLib test`')
        console.log();
        return;
      }
      console.log('library type: test');
      break;
    default:
      console.log('library type: unknown');
      console.log();
      return;
      break;
  }
  if (process.argv[2] == 'install') {
    const make = childProcess.spawnSync('make', ['-C', directoryInUse + file + '/']);
    if (make.status != 0) {
      console.log('failed to make library, verify the lib!')
      console.log();
      return;
    }
    const makeInstall = childProcess.spawnSync('make', ['-C', directoryInUse + file + '/', 'install']);
    if (makeInstall.status != 0) {
      console.log('failed to install library, verify the lib!')
      console.log();
      return;
    }
    console.log('library installed!');
    console.log();
  } else if (process.argv[2] == 'reinstall') {
    const make = childProcess.spawnSync('make', ['-C', directoryInUse + file + '/', 're']);
    if (make.status != 0) {
      console.log(make.stdout.toString('ascii'));
      console.log(make.stderr.toString('ascii'));
      console.log('failed to make library, verify the lib!')
      console.log();
      return;
    }
    const makeInstall = childProcess.spawnSync('make', ['-C', directoryInUse + file + '/', 'install']);
    if (makeInstall.status != 0) {
      console.log('failed to install library, verify the lib!')
      console.log();
      return;
    }
    console.log('library reinstalled!');
    console.log();
  } else if (process.argv[2] == 'uninstall') {
    const makeFclean = childProcess.spawnSync('make', ['-C', directoryInUse + file + '/', 'fclean']);
    if (makeFclean.status != 0) {
      console.log('failed to fclean library, verify the lib!')
      console.log();
      return;
    }
    console.log('library uninstalled!');
    console.log();
  }
};

const getLib = (files) => {
  files.forEach((file) => {
    console.log(directoryInUse);
    const fileStat = fs.statSync(directoryInUse + file);
    if (fileStat.isDirectory()) {
      buildLib(file);
    }
  });
};

// main call
directoryInUse = env.opaqueDir;
getLib(fs.readdirSync(env.opaqueDir));
directoryInUse = env.sharedDir;
getLib(fs.readdirSync(env.sharedDir));
directoryInUse = env.libDir;
getLib(fs.readdirSync(env.libDir));
