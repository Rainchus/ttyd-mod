.global StartCrashHandlerScale
.global BranchBackCrashHandlerScale

StartCrashHandlerScale:
bl scaleCrashHandlerText

BranchBackCrashHandlerScale:
b 0