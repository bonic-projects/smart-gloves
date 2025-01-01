import 'package:flutter_test/flutter_test.dart';
import 'package:smart_glove/app/app.locator.dart';

import '../helpers/test_helpers.dart';

void main() {
  group('FeedbackViewModel Tests -', () {
    setUp(() => registerServices());
    tearDown(() => locator.reset());
  });
}
