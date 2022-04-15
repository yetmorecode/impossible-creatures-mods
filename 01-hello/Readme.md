# Minimal Impossible Creatures Mod using the SDK

A mod has to be build as a DLL exposing the following interface hierarchy to the game:

* DLLInterface (HelloMod)
  * DLLGameInterface (HelloGame)
    * DLLCpuInterface (HelloCpu)
    * DLLGuiInterface (HelloGui)
    * DLLSimInterface (HelloSim)
  * DLLSetupInterface (HelloSetup)
  * DLLScoreInterface (HelloScore)

The goal of this mod is to provide a minimal implementation all of those and to be buildable on a modern 2022 toolchain (e.g. Visual Studio 2022 CE).
