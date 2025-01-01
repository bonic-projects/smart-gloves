import 'package:stacked/stacked.dart';
import 'package:flutter_tts/flutter_tts.dart';

class FeedbackViewModel extends BaseViewModel {
  final FlutterTts _tts = FlutterTts();

  String? _visualFeedback;
  String? get visualFeedback => _visualFeedback;

  Function(String)? _onDataReceived;

  FeedbackViewModel() {
    _onDataReceived = (data) {
      _visualFeedback = "Gesture Detected: $data";
      notifyListeners();
      _speak(data); // Announce the data via TTS
    };
  }

  Future<void> startListening() async {
    setBusy(true);
    await _tts.setLanguage("en-US");
    await _tts.setSpeechRate(0.5);
    // Ensure this listens to incoming data from the service
    // Replace with your service's real method to listen to data
    Future.delayed(Duration(seconds: 1), () {
      _onDataReceived?.call("Sample Gesture"); // Mock data for testing
    });
    setBusy(false);
  }

  Future<void> _speak(String message) async {
    await _tts.speak(message);
  }

  void stopListening() {
    _visualFeedback = null;
    notifyListeners();
  }

  @override
  void dispose() {
    stopListening();
    super.dispose();
  }
}
