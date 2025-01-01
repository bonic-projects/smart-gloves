import 'package:flutter/material.dart';
import 'package:stacked/stacked.dart';
import 'feedback_viewmodel.dart';

class FeedbackView extends StatelessWidget {
  final String connectedDeviceName;

  FeedbackView({required this.connectedDeviceName});

  @override
  Widget build(BuildContext context) {
    return ViewModelBuilder<FeedbackViewModel>.reactive(
      viewModelBuilder: () => FeedbackViewModel(),
      onModelReady: (viewModel) {
        viewModel.startListening(); // Start listening to data
      },
      builder: (context, viewModel, child) {
        return Scaffold(
          appBar: AppBar(
            title: Text('Visual Feedback - $connectedDeviceName'),
          ),
          body: Center(
            child: viewModel.isBusy
                ? CircularProgressIndicator()
                : viewModel.visualFeedback != null
                    ? Column(
                        mainAxisAlignment: MainAxisAlignment.center,
                        children: [
                          Text(
                            viewModel.visualFeedback!,
                            style: TextStyle(
                                fontSize: 24, fontWeight: FontWeight.bold),
                          ),
                          SizedBox(height: 20),
                          ElevatedButton(
                            onPressed: viewModel.stopListening,
                            child: Text('Stop Listening'),
                          ),
                        ],
                      )
                    : Text(
                        'Waiting for data...',
                        style: TextStyle(fontSize: 18),
                      ),
          ),
        );
      },
    );
  }
}
