#!/usr/bin/env node

const env = {
  libDir: 'assets/extern_lib/',
  configFileName: 'lib_config.json',
};

// let finalError = []; we should recap at the end if there is some errors

const fs = require('fs');
const childProcess = require('child_process');

const buildLib = (file) => {
  try {
    fs.accessSync(env.libDir + file + '/' + env.configFileName);
  } catch (err) {
    return;
  }
  const lib = JSON.parse(fs.readFileSync(env.libDir + file + '/' + env.configFileName));

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
    const make = childProcess.spawnSync('make', ['-C', env.libDir + file + '/']);
    if (make.status != 0) {
      console.log('failed to make library, verify the lib!')
      console.log();
      return;
    }
    const makeInstall = childProcess.spawnSync('make', ['-C', env.libDir + file + '/', 'install']);
    if (makeInstall.status != 0) {
      console.log('failed to install library, verify the lib!')
      console.log();
      return;
    }
    console.log('library installed!');
    console.log();
  } else if (process.argv[2] == 'uninstall') {
    const makeFclean = childProcess.spawnSync('make', ['-C', env.libDir + file + '/', 'fclean']);
    if (makeFclean.status != 0) {
      console.log('failed to fclean library, verify the lib!')
      console.log();
      return;
    }
    console.log('library uninstalled!');
    console.log();
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
