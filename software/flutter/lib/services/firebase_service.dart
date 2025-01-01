import 'package:firebase_database/firebase_database.dart';

final dbCode = "3c6FsvRdvIaObUmMh4byYeBbAIU2";

class FirebaseService {
  final _database = FirebaseDatabase.instance.ref();

  Stream<String> fetchData() {
    return _database
        .child('devices/$dbCode/prediction/gesture')
        .onValue
        .map((event) {
      final data = event.snapshot.value;
      return data.toString();
    });
  }
}
