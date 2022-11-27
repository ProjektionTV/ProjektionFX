Import("env", "projenv")
import datetime
import os

# collect version time, hash and version
buildtimestring = datetime.datetime.now().strftime("%d.%b.%Y %X")
githash = os.environ.get('GITHUB_SHA') or "0"*40
gitversion = os.environ.get('GITHUB_REF_NAME') or "manual build"

# define content of build_infos.h
build_infos_h = f"""
#ifndef __build_infos__
#define __build_infos__

// !!! THIS FILE IS GENERATED - DO NOT MODIFY !!!

class BuildInfos {{
  public:
    const char* getBuildTime(){{
      return "{buildtimestring}";
    }}
    const char* getGitHash(){{
      return "{githash}";
    }}
    const char* getGitVersion(){{
      return "{gitversion}";
    }}
}};

BuildInfos Build;

#endif
"""

# write file
with open('src/build_infos.h', 'w') as f:
  f.write(build_infos_h)

# output info on build console
print("/-----------------------")
print("| generated build infos")
print(f"| {buildtimestring}")
print(f"| {githash}")
print(f"| {gitversion}")
print("\\-----------------------")
