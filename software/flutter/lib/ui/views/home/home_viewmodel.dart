import 'package:flutter_tts/flutter_tts.dart';
import 'package:smart_glove/services/firebase_service.dart';
import 'package:speech_to_text/speech_to_text.dart';
import 'package:stacked/stacked.dart';

class HomeViewModel extends ReactiveViewModel {
  final FirebaseService _firebaseService = FirebaseService();
  final FlutterTts _flutterTts = FlutterTts();
  final SpeechToText _speechToText = SpeechToText();

  String _firebaseData = '';
  String _userSpeech = '';
  bool _isListening = false;

  String get firebaseData => _firebaseData;
  String _currentWord = '';
  String _displayedText = '';
  String get userSpeech => _userSpeech;
  bool get isListening => _isListening;

  String get displayedText => _displayedText;

  void initialize() {
    print("Initializing Firebase stream...");
    _firebaseService.fetchData().listen((data) {
      print("Fetched data from Firebase: $data");
      _processIncomingData(data);
    });
  }

  void _processIncomingData(String data) {
    final parts = data.split(' '); // Split the data by spaces

    for (var part in parts) {
      if (part == 'space') {
        // Detected the word "space"
        _speakText(_currentWord);
        _currentWord = ''; // Reset the current word
        _displayedText += ' '; // Add a visible space in the displayed text
      } else {
        // Add the part to the current word and display
        _currentWord += part;
        _displayedText += part;
      }
      notifyListeners();
    }
  }

  Future<void> _speakText(String text) async {
    if (text.isNotEmpty) {
      print("Speaking: $text");
      await _flutterTts.speak(text);
    }
  }

  Future<void> _clearSpeech() async {
    print("Clearing previous speech...");
    await _flutterTts.stop();
  }

  Future<void> listenToUser() async {
    print("Trying to initialize speech recognition...");
    bool available = await _speechToText.initialize();
    if (available) {
      print("Speech recognition initialized successfully.");
      if (!_isListening) {
        print("Starting to listen...");
        _isListening = true;
        _speechToText.listen(onResult: (result) {
          print("Speech recognized: ${result.recognizedWords}");
          _userSpeech = result.recognizedWords;
          notifyListeners();
        });
      }
    } else {
      print("Speech recognition is not available.");
    }
  }

  void stopListening() {
    if (_isListening) {
      print("Stopping speech recognition...");
      _isListening = false;
      _speechToText.stop();
      notifyListeners();
    }
  }

  void clearUserspeech() {
    _userSpeech = '';
    _speechToText.stop();
    notifyListeners();
  }

  void clearFirebaseData() {
    print("Clearing Firebase data...");
    _displayedText = '';
    _flutterTts.stop();
    notifyListeners();
  }

  @override
  List<ReactiveServiceMixin> get reactiveServices => [];
}
