#ifndef __PRINT_BUILD_INFOS__
#define __PRINT_BUILD_INFOS__

#include <Arduino.h>
#include <build_infos.h> // will be created while building the project

void printBuildInfos(){
  Serial.println();
  Serial.println();
  Serial.println();

  Serial.println("______                 _        _     _    _               ______ __   __");
  Serial.println("| ___ \\               (_)      | |   | |  (_)              |  ___|\\ \\ / /");
  Serial.println("| |_/ / _ __   ___     _   ___ | | __| |_  _   ___   _ __  | |_    \\ V / ");
  Serial.println("|  __/ | '__| / _ \\   | | / _ \\| |/ /| __|| | / _ \\ | '_ \\ |  _|   /   \\ ");
  Serial.println("| |    | |   | (_) |  | ||  __/|   < | |_ | || (_) || | | || |    / /^\\ \\");
  Serial.println("\\_|    |_|    \\___/   | | \\___||_|\\_\\ \\__||_| \\___/ |_| |_|\\_|    \\/   \\/");
  Serial.println("                     _/ |                                                ");
  Serial.println("                    |__/                                                 ");

  Serial.println();
  Serial.println("https://github.com/ProjektionTV/ProjektionFX");

  Serial.println();
  Serial.printf("VERSION:    %s\n", Build.getGitVersion());
  Serial.printf("GIT HASH:   %s\n", Build.getGitHash());
  Serial.printf("BUILD DATE: %s\n", Build.getBuildTime());
  Serial.println();
}

#endif
