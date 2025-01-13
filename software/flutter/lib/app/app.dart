import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:smart_glove/ui/bottom_sheets/notice/notice_sheet.dart';
import 'package:smart_glove/ui/dialogs/info_alert/info_alert_dialog.dart';
import 'package:smart_glove/ui/views/home/home_view.dart';
import 'package:smart_glove/ui/views/startup/startup_view.dart';
import 'package:stacked/stacked_annotations.dart';
import 'package:stacked_services/stacked_services.dart';
import 'package:smart_glove/services/firebase_service.dart';

// @stacked-import

@StackedApp(routes: [
  MaterialRoute(page: HomeView),
  MaterialRoute(page: StartupView),

// @stacked-route
], dependencies: [
  LazySingleton(classType: BottomSheetService),
  LazySingleton(classType: DialogService),
  LazySingleton(classType: NavigationService),
  LazySingleton(classType: FirebaseService),
// @stacked-service
], bottomsheets: [
  StackedBottomsheet(classType: NoticeSheet),
  // @stacked-bottom-sheet
], dialogs: [
  StackedDialog(classType: InfoAlertDialog),
  // @stacked-dialog
], logger: StackedLogger())
class App {}
