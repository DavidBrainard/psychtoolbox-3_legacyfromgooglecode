% list of m-files that come with PTB, current as of revision 2302,
% September 19th, 2011.
%
% for each m-file, the function that tests it is listed, if any
%
% generate with:
%   str = DirList(PsychToolBoxRoot,true,[],[],'.svn|.FBCLockFolder|PsychHardware|PsychOpenGL|MOAL|PsychDocumentation|PsychDemos|PsychTests|PsychObsolete|PsychMatlabTests','\.m$',true)
% and finally some manual editing
%
%
% PsychAlpha/BlurredVideoCaptureDemo.m
% PsychAlpha/FastSeparableFilteredNoiseDemo.m
% PsychAlpha/ImageUndistortionDemo.m
% PsychAlpha/ImageWarpingDemo.m
% PsychAlpha/OpenEyesDemo.m
% PsychAlpha/PrintStruct.m
% PsychAlpha/StoreBit.m
% PsychAlphaBlending/AlphaBlend.m
% PsychAlphaBlending/AlphaDestinationTerm.m
% PsychAlphaBlending/AlphaSourceTerm.m
% PsychAlphaBlending/AlphaSum.m
% PsychAlphaBlending/AlphaTimes.m
% PsychAlphaBlending/GL_DST_ALPHA.m
% PsychAlphaBlending/GL_DST_COLOR.m
% PsychAlphaBlending/GL_ONE.m
% PsychAlphaBlending/GL_ONE_MINUS_DST_ALPHA.m
% PsychAlphaBlending/GL_ONE_MINUS_DST_COLOR.m
% PsychAlphaBlending/GL_ONE_MINUS_SRC_ALPHA.m
% PsychAlphaBlending/GL_ONE_MINUS_SRC_COLOR.m
% PsychAlphaBlending/GL_SRC_ALPHA.m
% PsychAlphaBlending/GL_SRC_ALPHA_SATURATE.m
% PsychAlphaBlending/GL_SRC_COLOR.m
% PsychAlphaBlending/GL_ZERO.m
% PsychAlphaBlending/MaskImageIn.m
% PsychAlphaBlending/MaskImageOut.m
% PsychAlphaBlending/SetDestinationAlpha.m
% PsychAlphaBlending/SetImageAlpha.m
% PsychAlphaBlending/SetSourceAlpha.m
% PsychBasic/Beeper.m
% PsychBasic/CharAvail.m
% PsychBasic/DisableKeysForKbCheck.m
% PsychBasic/DoNothing.m
% PsychBasic/DrawFormattedText.m
% PsychBasic/FlushEvents.m
% PsychBasic/FontInfo.m
% PsychBasic/FrameRateFromMeasurement.m
% PsychBasic/Gestalt.m
% PsychBasic/GetBusTicks.m
% PsychBasic/GetBusTicksTick.m
% PsychBasic/GetChar.m
% PsychBasic/GetClicks.m
% PsychBasic/GetMouse.m
% PsychBasic/GetMouseWheel.m
% PsychBasic/GetPID.m
% PsychBasic/GetSecs.m
% PsychBasic/GetSecsTick.m
% PsychBasic/GetTicks.m
% PsychBasic/GetTicksTick.m
% PsychBasic/HideCursor.m
% PsychBasic/IOPort.m
% PsychBasic/InitCocoaEventBridge.m
% PsychBasic/KbCheck.m
% PsychBasic/KbKeysAction.m
% PsychBasic/KbName.m
% PsychBasic/KbPressWait.m
% PsychBasic/KbQueueCheck.m
% PsychBasic/KbQueueCreate.m
% PsychBasic/KbQueueFlush.m
% PsychBasic/KbQueueRelease.m
% PsychBasic/KbQueueStart.m
% PsychBasic/KbQueueStop.m
% PsychBasic/KbQueueWait.m
% PsychBasic/KbReleaseWait.m
% PsychBasic/KbStrokeWait.m
% PsychBasic/KbTriggerWait.m
% PsychBasic/KbWait.m
% PsychBasic/ListenChar.m
% PsychBasic/LoadClut.m
% PsychBasic/LoadPsychHID.m
% PsychBasic/MachAbsoluteTimeClockFrequency.m
% PsychBasic/PredictVisualOnsetForTime.m
% PsychBasic/PsychCV.m
% PsychBasic/PsychDrawSprites2D.m
% PsychBasic/PsychKinect.m
% PsychBasic/PsychKinectCore.m
% PsychBasic/PsychPortAudio.m
% PsychBasic/PsychSerial.m
% PsychBasic/PsychWatchDog.m
% PsychBasic/PsychtoolboxDate.m
% PsychBasic/PsychtoolboxRegistration.m
% PsychBasic/PsychtoolboxVersion.m
% PsychBasic/RestrictKeysForKbCheck.m
% PsychBasic/Screen.m
% PsychBasic/ScreenDrawDots.m
% PsychBasic/SetMouse.m
% PsychBasic/ShowCursor.m
% PsychBasic/Snd.m
% PsychBasic/VideoRefreshFromMeasurement.m
% PsychBasic/WaitSecs.m
% PsychBasic/WaitTicks.m
% PsychBasic/clearGetSecs.m
% PsychBasic/clearIOPort.m
% PsychBasic/clearPsychHID.m
% PsychBasic/clearPsychPortAudio.m
% PsychBasic/clearScreen.m
% PsychBasic/clearWaitSecs.m
% PsychBasic/clearall.m
% PsychBasic/clearmex.m
% PsychBasic/clearmoglcore.m
% PsychBasic/psychassert.m
% PsychBasic/psychlasterror.m
% PsychBasic/psychrethrow.m
% PsychCal/CVIAdjustCalData.m
% PsychCal/CalDataFolder.m
% PsychCal/CalFormatToImage.m
% PsychCal/CalToggleBitsPlusPlus.m
% PsychCal/CalibrateAmbDrvr.m
% PsychCal/CalibrateFitGamma.m
% PsychCal/CalibrateFitLinMod.m
% PsychCal/CalibrateFitYoked.m
% PsychCal/CalibrateMonDrvr.m
% PsychCal/CalibrateMonSpd.m
% PsychCal/CalibrateMonitorPhotometer.m
% PsychCal/CalibratePlotAmbient.m
% PsychCal/CalibratePlotGamma.m
% PsychCal/CalibratePlotSpectra.m
% PsychCal/CompareMonCal.m
% PsychCal/CompareMonCalOverTime.m
% PsychCal/ContrastMatch.m
% PsychCal/Cross2D.m
% PsychCal/Cross3D.m
% PsychCal/CylToSensor.m
% PsychCal/DescribeMonCal.m
% PsychCal/DropCalBits.m
% PsychCal/DumpMonCalSpd.m
% PsychCal/ExpandSettings.m
% PsychCal/FindSpectralPeaks.m
% PsychCal/FlushCalFile.m
% PsychCal/GamutToSettings.m
% PsychCal/GamutToSettingsSch.m
% PsychCal/GamutToSettingsTbl.m
% PsychCal/GratingNull.m
% PsychCal/IccProfile.m
% PsychCal/ImageToCalFormat.m
% PsychCal/InvertGammaTable.m
% PsychCal/LoadCalFile.m
% PsychCal/MaximizeGamutContrast.m
% PsychCal/MeasMonSpd.m
% PsychCal/MeasMonXYZ.m
% PsychCal/MeasureDpi.m
% PsychCal/NotYetUpdatedForPTB3/CalibrateManualDrvr.m
% PsychCal/PolarToSensor.m
% PsychCal/PrimaryToGamut.m
% PsychCal/PrimaryToSensor.m
% PsychCal/PrimaryToSettings.m
% PsychCal/RefitCalGamma.m
% PsychCal/RefitCalLinMod.m
% PsychCal/RefitCalYoked.m
% PsychCal/SaveCalFile.m
% PsychCal/SearchGammaTable.m
% PsychCal/SensorToCyl.m
% PsychCal/SensorToPolar.m
% PsychCal/SensorToPrimary.m
% PsychCal/SensorToSettings.m
% PsychCal/SensorToSettingsAcc.m
% PsychCal/SetGammaMethod.m
% PsychCal/SetSensorColorSpace.m
% PsychCal/SettingsToPrimary.m
% PsychCal/SettingsToSensor.m
% PsychCal/SettingsToSensorAcc.m
% PsychCal/UpdateAmbient.m
% PsychCal/UpdateAmbientSensor.m
% PsychColorimetric/AbsorbanceToAbsorbtance.m
% PsychColorimetric/CheckWls.m
% PsychColorimetric/ComputeAxialDensity.m
% PsychColorimetric/ComputeDE.m
% PsychColorimetric/ComputeDKL_M.m
% PsychColorimetric/ConeIncToDKL.m
% PsychColorimetric/ContrastToExcitation.m
% PsychColorimetric/ContrastToIncrement.m
% PsychColorimetric/DKLToConeInc.m
% PsychColorimetric/EffectiveTrolandsFromLum.m
% PsychColorimetric/EnergyToCones.m
% PsychColorimetric/EnergyToQuanta.m
% PsychColorimetric/ExcitationToContrast.m
% PsychColorimetric/FToRatio.m
% PsychColorimetric/FindCovLinMod.m
% PsychColorimetric/FindLinMod.m
% PsychColorimetric/FindModelWeights.m
% PsychColorimetric/GenerateBlackBody.m
% PsychColorimetric/GenerateCIEDay.m
% PsychColorimetric/IncrementToContrast.m
% PsychColorimetric/IsomerizationsFromAbsorptions.m
% PsychColorimetric/LMSToMacBoyn.m
% PsychColorimetric/LabToXYZ.m
% PsychColorimetric/LjgToXYZ.m
% PsychColorimetric/LjgToXYZFun.m
% PsychColorimetric/LumToRadiance.m
% PsychColorimetric/LumToRetIrradiance.m
% PsychColorimetric/LumToTrolands.m
% PsychColorimetric/LuvToXYZ.m
% PsychColorimetric/LxxToY.m
% PsychColorimetric/M_PToP.m
% PsychColorimetric/M_PToT.m
% PsychColorimetric/M_TToP.m
% PsychColorimetric/M_TToT.m
% PsychColorimetric/MakeFourierBasis.m
% PsychColorimetric/MakeGaussBasis.m
% PsychColorimetric/MakeItS.m
% PsychColorimetric/MakeItStruct.m
% PsychColorimetric/MakeItWls.m
% PsychColorimetric/MakeMonoPrimary.m
% PsychColorimetric/MakeOrtho.m
% PsychColorimetric/MakeUnitLength.m
% PsychColorimetric/MonoImageToSRGB.m
% PsychColorimetric/PhotonAbsorptionRate.m
% PsychColorimetric/PowerToTrolands.m
% PsychColorimetric/PsychMunsell/MunsellAngleToHue.m
% PsychColorimetric/PsychMunsell/MunsellConversionTest.m
% PsychColorimetric/PsychMunsell/MunsellGetxyY.m
% PsychColorimetric/PsychMunsell/MunsellGriddata3.m
% PsychColorimetric/PsychMunsell/MunsellHueToAngle.m
% PsychColorimetric/PsychMunsell/MunsellPreprocessTable.m
% PsychColorimetric/QuantaToEnergy.m
% PsychColorimetric/QuantaToTrolands.m
% PsychColorimetric/RadianceToRetIrradiance.m
% PsychColorimetric/RetIrradianceToIsoRecSec.m
% PsychColorimetric/RetIrradianceToTrolands.m
% PsychColorimetric/SPDToLinSPD.m
% PsychColorimetric/SPDToMetSPD.m
% PsychColorimetric/SRGBGammaCorrect.m
% PsychColorimetric/SRGBGammaUncorrect.m
% PsychColorimetric/SRGBPrimaryToXYZ.m
% PsychColorimetric/SToWls.m
% PsychColorimetric/SampleCircle.m
% PsychColorimetric/SampleSphere.m
% PsychColorimetric/ShiftSpectra.m
% PsychColorimetric/SplineCmf.m
% PsychColorimetric/SplineRaw.m
% PsychColorimetric/SplineSpd.m
% PsychColorimetric/SplineSrf.m
% PsychColorimetric/TriToMetSPD.m
% PsychColorimetric/TrolandsToLum.m
% PsychColorimetric/TrolandsToPower.m
% PsychColorimetric/TrolandsToQuanta.m
% PsychColorimetric/TrolandsToRetIrradiance.m
% PsychColorimetric/WattsToRetIrradiance.m
% PsychColorimetric/WlsToS.m
% PsychColorimetric/WlsToT.m
% PsychColorimetric/XYZToF.m
% PsychColorimetric/XYZToLab.m
% PsychColorimetric/XYZToLjg.m
% PsychColorimetric/XYZToLuv.m
% PsychColorimetric/XYZToSRGBPrimary.m
% PsychColorimetric/XYZTouv.m
% PsychColorimetric/XYZTouvY.m
% PsychColorimetric/XYZToxyY.m
% PsychColorimetric/uvTols.m
% PsychColorimetric/uvToxy.m
% PsychColorimetric/uvYToXYZ.m
% PsychColorimetric/xyTouv.m
% PsychColorimetric/xyYToXYZ.m
% PsychColorimetricData/BaylorNomogram.m
% PsychColorimetricData/ComputeCIEConeFundamentals.m
% PsychColorimetricData/ComputeRawConeFundamentals.m
% PsychColorimetricData/DawisNomogram.m
% PsychColorimetricData/DefaultPhotoreceptors.m
% PsychColorimetricData/DegreesToRetinalMM.m
% PsychColorimetricData/EyeLength.m
% PsychColorimetricData/FillInPhotoreceptors.m
% PsychColorimetricData/FitConeFundamentalsWithNomogram.m
% PsychColorimetricData/GovardovskiiNomogram.m
% PsychColorimetricData/LambNomogram.m
% PsychColorimetricData/LensTransmittance.m
% PsychColorimetricData/MacularTransmittance.m
% PsychColorimetricData/PhotopigmentAxialDensity.m
% PsychColorimetricData/PhotopigmentNomogram.m
% PsychColorimetricData/PhotopigmentQuantalEfficiency.m
% PsychColorimetricData/PhotopigmentSpecificDensity.m
% PsychColorimetricData/PhotoreceptorDimensions.m
% PsychColorimetricData/PupilDiameterFromLum.m
% PsychColorimetricData/RetinalMMToDegrees.m
% PsychColorimetricData/StockmanSharpeNomogram.m
% PsychColorimetricData/ValetonVanNorrenParams.m
% PsychContributed/WinTab/wintabfastLoop.m
% PsychContributed/WinTab/wintabslowLoop.m
% PsychContributed/ple.m
% PsychContributed/read_rle_rgbe.m
% PsychFiles/BreakLines.m
% PsychFiles/CheckDirs.m
% PsychFiles/DirList.m
% PsychFiles/FileFromFolder.m                                             - FileFromFolderTests.m
% PsychFiles/FindFolder.m
% PsychFiles/FolderFromFolder.m                                           - FolderFromFolder.m
% PsychFiles/LiteralUnderscore.m
% PsychFiles/LogVar.m
% PsychFiles/PsychHomeDir.m
% PsychFiles/PsychSaveAsEps.m
% PsychFiles/PsychtoolboxConfigDir.m
% PsychFiles/QuoteString.m
% PsychFiles/ReadStructsFromText.m
% PsychFiles/ReplaceLineTerminators.m
% PsychFiles/Var2Str.m                                                    - FileVar2Str.m
% PsychFiles/WriteStructsToText.m
% PsychGLImageProcessing/Add2DConvolutionToGLOperator.m
% PsychGLImageProcessing/Add2DSeparableConvolutionToGLOperator.m
% PsychGLImageProcessing/AddImageUndistortionToGLOperator.m
% PsychGLImageProcessing/AddImageWarpToGLOperator.m
% PsychGLImageProcessing/AddToGLOperator.m
% PsychGLImageProcessing/BitsPlusPlus.m
% PsychGLImageProcessing/CountSlotsInGLOperator.m
% PsychGLImageProcessing/CreateDisplayWarp.m
% PsychGLImageProcessing/CreateGLOperator.m
% PsychGLImageProcessing/CreateProceduralGabor.m
% PsychGLImageProcessing/CreateProceduralSineGrating.m
% PsychGLImageProcessing/CreatePseudoGrayLUT.m
% PsychGLImageProcessing/CreateSinglePassImageProcessingShader.m
% PsychGLImageProcessing/DisplayUndistortionBVL.m
% PsychGLImageProcessing/DisplayUndistortionBezier.m
% PsychGLImageProcessing/DisplayUndistortionHalfCylinder.m
% PsychGLImageProcessing/DisplayUndistortionSphere.m
% PsychGLImageProcessing/HookProcessingChains.m
% PsychGLImageProcessing/MakeTextureDrawShader.m
% PsychGLImageProcessing/PsychColorCorrection.m
% PsychGLImageProcessing/PsychHelperCreateARGB2101010RemapCLUT.m
% PsychGLImageProcessing/PsychHelperCreateGenericLuminanceToRGBA8LUT.m
% PsychGLImageProcessing/PsychHelperCreateRemapCLUT.m
% PsychGLImageProcessing/PsychImaging.m
% PsychGLImageProcessing/PsychVideoSwitcher.m
% PsychGLImageProcessing/SetAnaglyphStereoParameters.m
% PsychGLImageProcessing/SetStereoBlueLineSyncParameters.m
% PsychGLImageProcessing/VignetCalibration.m
% PsychGLImageProcessing/kPsychAssumeTextureNormalized.m
% PsychGLImageProcessing/kPsychNeed16BPCFixed.m
% PsychGLImageProcessing/kPsychNeed16BPCFloat.m
% PsychGLImageProcessing/kPsychNeed32BPCFloat.m
% PsychGLImageProcessing/kPsychNeedDualPass.m
% PsychGLImageProcessing/kPsychNeedDualWindowOutput.m
% PsychGLImageProcessing/kPsychNeedFastBackingStore.m
% PsychGLImageProcessing/kPsychNeedFastOffscreenWindows.m
% PsychGLImageProcessing/kPsychNeedHalfHeightWindow.m
% PsychGLImageProcessing/kPsychNeedHalfWidthWindow.m
% PsychGLImageProcessing/kPsychNeedImageProcessing.m
% PsychGLImageProcessing/kPsychNeedMultiPass.m
% PsychGLImageProcessing/kPsychNeedOutputConversion.m
% PsychGLImageProcessing/kPsychUse32BPCFloatAsap.m
% PsychGLImageProcessing/moglExtractTexture.m
% PsychGLImageProcessing/private/BFRenderSceneUnwrap.m
% PsychGLImageProcessing/private/DrawRectFrame.m
% PsychGLImageProcessing/private/NewCalibFile.m
% PsychGLImageProcessing/private/bvlAcceptCalibGuiInput.m
% PsychGLImageProcessing/private/bvlBarrelPincushionDots.m
% PsychGLImageProcessing/private/bvlDrawCrosshair.m
% PsychGLImageProcessing/private/bvlDrawText.m
% PsychGLImageProcessing/private/bvlExpandContractDots.m
% PsychGLImageProcessing/private/bvlSelectFitPts.m
% PsychGLImageProcessing/private/bvlShearDots.m
% PsychGLImageProcessing/private/bvlTranslateDots.m
% PsychGLImageProcessing/private/bvlTrapezoidDots.m
% PsychGLImageProcessing/private/bvlWaitForInput.m
% PsychGLImageProcessing/private/createnewcalibrationgrid.m
% PsychGLImageProcessing/private/fineadjust.m
% PsychGLImageProcessing/private/fit.m
% PsychGLImageProcessing/private/manual_calibrate.m
% PsychGLImageProcessing/private/poly2fit.m
% PsychGLImageProcessing/private/selectcalibrationpoints.m
% PsychGamma/CheckMonotonic.m
% PsychGamma/ComputeGammaExtP.m
% PsychGamma/ComputeGammaPoly.m
% PsychGamma/ComputeGammaPolyR.m
% PsychGamma/ComputeGammaPow.m
% PsychGamma/ComputeGammaSig.m
% PsychGamma/ComputeGammaWeib.m
% PsychGamma/ComputeRMSE.m
% PsychGamma/FitDeviceGamma.m
% PsychGamma/FitGamma.m
% PsychGamma/FitGammaExtP.m
% PsychGamma/FitGammaExtPFun.m
% PsychGamma/FitGammaLin.m
% PsychGamma/FitGammaPoly.m
% PsychGamma/FitGammaPolyFun.m
% PsychGamma/FitGammaPolyR.m
% PsychGamma/FitGammaPow.m
% PsychGamma/FitGammaPowFun.m
% PsychGamma/FitGammaSig.m
% PsychGamma/FitGammaSigFun.m
% PsychGamma/FitGammaSpline.m
% PsychGamma/FitGammaWeib.m
% PsychGamma/FitGammaWeibFun.m
% PsychGamma/HalfRect.m
% PsychGamma/InitialXExtP.m
% PsychGamma/InitialXPoly.m
% PsychGamma/InitialXPolyR.m
% PsychGamma/InitialXPow.m
% PsychGamma/InitialXSig.m
% PsychGamma/InitialXWeib.m
% PsychGamma/InvertGammaExtP.m
% PsychGamma/MakeGammaMonotonic.m
% PsychGamma/MakeMonotonic.m
% PsychGamma/MonitorGamma.m
% PsychGamma/MonitorGammaError.m
% PsychGamma/NormalizeGamma.m
% PsychInitialize/IsStartUpdateImplantedInFinish.m
% PsychInitialize/finish.m
% PsychJava/AddPsychJavaPath.m
% PsychJava/AssignGetCharJava.m
% PsychJava/IsPsychJavaPathSet.m
% PsychJava/JavaTimeToGetSecs.m
% PsychJava/PsychJavaTrouble.m
% PsychJava/psychusejava.m
% PsychOneliners/AddStructs.m
% PsychOneliners/AltSize.m
% PsychOneliners/AppleVersion.m
% PsychOneliners/AreStructsEqualOnFields.m
% PsychOneliners/Ask.m
% PsychOneliners/AssertGLSL.m
% PsychOneliners/AssertMex.m
% PsychOneliners/AssertOSX.m
% PsychOneliners/AssertOpenGL.m
% PsychOneliners/BackupCluts.m
% PsychOneliners/BlackIndex.m
% PsychOneliners/CatStr.m
% PsychOneliners/CenterMatOnPoint.m
% PsychOneliners/Circle.m
% PsychOneliners/CleanStruct.m
% PsychOneliners/CreateUniformDotsIn3DFrustum.m
% PsychOneliners/CropBlackEdges.m
% PsychOneliners/DeEmptify.m
% PsychOneliners/DescribeComputer.m
% PsychOneliners/DiskRoot.m
% PsychOneliners/DotOffset.m
% PsychOneliners/Ellipse.m
% PsychOneliners/EnforcePos.m
% PsychOneliners/Explode.m
% PsychOneliners/FillEmptyFields.m
% PsychOneliners/FindInd.m
% PsychOneliners/FindRepeatAlongDims.m
% PsychOneliners/FrameRate.m
% PsychOneliners/FunctionFolder.m
% PsychOneliners/GetEchoNumber.m
% PsychOneliners/GetEchoString.m
% PsychOneliners/GetKbChar.m
% PsychOneliners/GetMyCaller.m
% PsychOneliners/GetNumber.m
% PsychOneliners/GetString.m
% PsychOneliners/GetSubversionPath.m
% PsychOneliners/GetWithDefault.m
% PsychOneliners/GrayIndex.m
% PsychOneliners/GroupStructArrayByFields.m
% PsychOneliners/ImageToVec.m
% PsychOneliners/Ind2Str.m
% PsychOneliners/Interleave.m
% PsychOneliners/IsACell.m
% PsychOneliners/IsLinux.m
% PsychOneliners/IsMinimumOSXVersion.m
% PsychOneliners/IsOS9.m
% PsychOneliners/IsOSX.m
% PsychOneliners/IsOctave.m
% PsychOneliners/IsWin.m
% PsychOneliners/IsWinMatlabR11Style.m
% PsychOneliners/KbMapKey.m
% PsychOneliners/LoadIdentityClut.m
% PsychOneliners/MacModelName.m
% PsychOneliners/Magnify2DMatrix.m
% PsychOneliners/MakeBeep.m
% PsychOneliners/MakeCosImage.m
% PsychOneliners/MakeGrid.m
% PsychOneliners/MakeSincImage.m
% PsychOneliners/MakeSineImage.m
% PsychOneliners/MapIndexColorThroughClut.m
% PsychOneliners/MergeCell.m
% PsychOneliners/NameBytes.m
% PsychOneliners/NameFrequency.m
% PsychOneliners/NearestResolution.m
% PsychOneliners/OSName.m
% PsychOneliners/PackColorImage.m
% PsychOneliners/PathListIsMember.m
% PsychOneliners/ProgressBar.m
% PsychOneliners/PsychDebugWindowConfiguration.m
% PsychOneliners/PsychGPUControl.m
% PsychOneliners/PsychNumel.m
% PsychOneliners/PsychtoolboxRoot.m
% PsychOneliners/RemoveSVNPaths.m
% PsychOneliners/Replace.m
% PsychOneliners/Resolute.m
% PsychOneliners/RestoreCluts.m
% PsychOneliners/Rot3d.m
% PsychOneliners/SaveIdentityClut.m
% PsychOneliners/SaveMovieFrames.m
% PsychOneliners/ScreenDacBits.m
% PsychOneliners/SetResolution.m
% PsychOneliners/ShrinkMatrix.m
% PsychOneliners/SmartVec.m
% PsychOneliners/SortCell.m
% PsychOneliners/Speak.m
% PsychOneliners/Stopwatch.m
% PsychOneliners/StrPad.m
% PsychOneliners/Struct2Vect.m
% PsychOneliners/TextBounds.m
% PsychOneliners/TextCenteredBounds.m
% PsychOneliners/UnpackColorImage.m
% PsychOneliners/VecToImage.m
% PsychOneliners/WhiteIndex.m
% PsychOneliners/WinDesk.m
% PsychOneliners/WrapString.m
% PsychOneliners/hexstr.m
% PsychOneliners/kPsychDontDoRotation.m
% PsychOneliners/kPsychUseTextureMatrixForRotation.m
% PsychOneliners/log10nw.m
% PsychOneliners/sca.m
% PsychOneliners/streq.m
% PsychOptics/AiryPattern.m
% PsychOptics/ComputeDiffLimit.m
% PsychOptics/DiffractionMTF.m
% PsychOptics/GoodmanDiffrac.m
% PsychOptics/MakeRadiusMat.m
% PsychOptics/WestLSFMinutes.m
% PsychOptics/WestPSFDegrees.m
% PsychOptics/WestPSFMinutes.m
% PsychOptics/WilliamsMTF.m
% PsychOptics/WilliamsRestMTF.m
% PsychPriority/GetProcessDescriptorFromCommandName.m
% PsychPriority/GetProcessList.m
% PsychPriority/GetRawProcessList.m
% PsychPriority/IsUpdateRunning.m
% PsychPriority/KillUpdateProcess.m
% PsychPriority/MachGetPriorityFlavor.m
% PsychPriority/MachGetPriorityMex.m
% PsychPriority/MachSetPriorityMex.m
% PsychPriority/MachSetStandardPriority.m
% PsychPriority/MachSetTimeConstraintPriority.m
% PsychPriority/MaxPriority.m
% PsychPriority/Priority.m
% PsychPriority/Rush.m
% PsychPriority/StartUpdateProcess.m
% PsychProbability/BalanceFactors.m
% PsychProbability/BalanceTrials.m
% PsychProbability/BuildMarkovK.m
% PsychProbability/ChiSqrCumulative.m
% PsychProbability/ChiSquarePDF.m
% PsychProbability/ChooseKFromN.m
% PsychProbability/ClockRandSeed.m
% PsychProbability/CoinFlip.m
% PsychProbability/CoinFlipPDF.m
% PsychProbability/ColMean.m
% PsychProbability/ColStd.m
% PsychProbability/CombVec.m
% PsychProbability/CovToCorr.m
% PsychProbability/FindNeighborCorr.m
% PsychProbability/MultiNormalDraw.m
% PsychProbability/MultiNormalPDF.m
% PsychProbability/NRandPerm.m
% PsychProbability/NormalCumulative.m
% PsychProbability/NormalDraw.m
% PsychProbability/NormalPDF.m
% PsychProbability/NormalProb.m
% PsychProbability/RandDim.m
% PsychProbability/RandLim.m
% PsychProbability/RandSample.m
% PsychProbability/RandSel.m
% PsychProbability/Randi.m
% PsychProbability/Ranint.m
% PsychProbability/Sample.m
% PsychProbability/Shuffle.m
% PsychProbability/URandSel.m
% PsychProbability/UniqueAndN.m
% PsychProbability/UniqueNoSorting.m
% PsychRects/AdjoinRect.m
% PsychRects/AlignRect.m
% PsychRects/ArrangeRects.m
% PsychRects/CenterRect.m
% PsychRects/CenterRectOnPoint.m
% PsychRects/CenterRectOnPointd.m
% PsychRects/ClipRect.m
% PsychRects/GrowRect.m
% PsychRects/InsetRect.m
% PsychRects/IsEmptyRect.m
% PsychRects/IsInRect.m
% PsychRects/OffsetRect.m
% PsychRects/RectBottom.m
% PsychRects/RectCenter.m
% PsychRects/RectCenterd.m
% PsychRects/RectHeight.m
% PsychRects/RectLeft.m
% PsychRects/RectOfMatrix.m
% PsychRects/RectRight.m
% PsychRects/RectSize.m
% PsychRects/RectTop.m
% PsychRects/RectWidth.m
% PsychRects/ScaleRect.m
% PsychRects/SetRect.m
% PsychRects/SizeOfRect.m
% PsychRects/UnionRect.m
% PsychSignal/ATand2.m
% PsychSignal/ATand360.m
% PsychSignal/AllAll.m
% PsychSignal/AltRound.m
% PsychSignal/AnyAll.m
% PsychSignal/Bandpass.m
% PsychSignal/Bandpass2.m
% PsychSignal/BoolFromSNR.m
% PsychSignal/ComputeRMSE.m
% PsychSignal/Expand.m
% PsychSignal/HalfRect.m
% PsychSignal/MatMax.m
% PsychSignal/MatMin.m
% PsychSignal/MatMinMax.m
% PsychSignal/MaxAll.m
% PsychSignal/MaxInd.m
% PsychSignal/MeanAll.m
% PsychSignal/MinAll.m
% PsychSignal/MinInd.m
% PsychSignal/OrientationBandpass.m
% PsychSignal/Scale.m
% PsychSignal/StdAll.m
% PsychSignal/SumAll.m
% PsychSignal/Trunc.m
% PsychSound/InitializePsychSound.m
% PsychSound/moallowlatency.m
% PsychSound/palowlatency.m
% PsychSound/sndlowlatency.m
% PsychStairCase/FitCumGauss_MES.m
% PsychStairCase/MinExpEntStair.m
% PsychVideoCapture/PsychCamSettings.m
% PsychVideoCapture/PsychGetCamIdForSpec.m
% PsychVideoCapture/PsychOpenEyes.m
% PsychVideoCapture/PsychSetupCamera.m
% PsychVideoCapture/PsychVideoDelayLoop.m
% PsychVideoCapture/VideoDelayLoopMiniDemo.m
% Psychometric/AreaUnderROC.m
% Psychometric/ComputeLogistic.m
% Psychometric/ComputeNakaRushton.m
% Psychometric/ComputeOptimalBeta.m
% Psychometric/ComputeWeibTAFC.m
% Psychometric/ComputeWeibYN.m
% Psychometric/CumNormYNFitFun.m
% Psychometric/DbToLog.m
% Psychometric/FindAlphaWeibTAFC.m
% Psychometric/FindEpsWeibTAFC.m
% Psychometric/FindThreshWeibTAFC.m
% Psychometric/FindThreshWeibYN.m
% Psychometric/FitCumNormYN.m
% Psychometric/FitLogistic.m
% Psychometric/FitLogitYN.m
% Psychometric/FitNakaRushton.m
% Psychometric/FitNakaRushtonFun.m
% Psychometric/FitWeibAlphTAFC.m
% Psychometric/FitWeibTAFC.m
% Psychometric/FitWeibYN.m
% Psychometric/GiveFeedback.m
% Psychometric/InvertLogistic.m
% Psychometric/LogToDb.m
% Psychometric/NormLikelyRatio.m
% Psychometric/NormLogLikelyRatio.m
% Psychometric/NormalROC.m
% Psychometric/SimWeibTAFCObs.m
% Psychometric/WeibAlphTAFCFitFun.m
% Psychometric/WeibTAFCFitFun.m
% Psychometric/WeibYNFitFun.m
% PsychtoolboxPostInstallRoutine.m
% Quest/QuestBetaAnalysis.m
% Quest/QuestCreate.m
% Quest/QuestDemo.m
% Quest/QuestMean.m
% Quest/QuestMode.m
% Quest/QuestP.m
% Quest/QuestPdf.m
% Quest/QuestQuantile.m
% Quest/QuestRecompute.m
% Quest/QuestSd.m
% Quest/QuestSimulate.m
% Quest/QuestTrials.m
% Quest/QuestUpdate.m