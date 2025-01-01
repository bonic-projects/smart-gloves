import 'package:flutter/material.dart';
import 'package:smart_glove/ui/components/common_button.dart';
import 'package:smart_glove/ui/views/home/home_viewmodel.dart';
import 'package:stacked/stacked.dart';

class HomeView extends StatelessWidget {
  const HomeView({super.key});

  @override
  Widget build(BuildContext context) {
    return ViewModelBuilder<HomeViewModel>.reactive(
      viewModelBuilder: () => HomeViewModel(),
      onModelReady: (viewModel) => viewModel.initialize(),
      builder: (context, viewModel, child) {
        return Scaffold(
          appBar: AppBar(
            title: const Text(
              'Smart Glove',
              style: TextStyle(fontWeight: FontWeight.bold),
            ),
            centerTitle: true,
            backgroundColor: Colors.teal,
            elevation: 5,
          ),
          body: Container(
            height: MediaQuery.of(context).size.height,
            width: MediaQuery.of(context).size.width,
            decoration: const BoxDecoration(
              gradient: LinearGradient(
                colors: [Colors.teal, Colors.tealAccent],
                begin: Alignment.topCenter,
                end: Alignment.bottomCenter,
              ),
            ),
            child: Padding(
              padding: const EdgeInsets.all(20.0),
              child: Column(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                crossAxisAlignment: CrossAxisAlignment.center,
                children: [
                  const Text(
                    'Text Display & Speech',
                    style: TextStyle(
                      fontSize: 24,
                      fontWeight: FontWeight.bold,
                      color: Colors.white,
                      letterSpacing: 1.2,
                    ),
                    textAlign: TextAlign.center,
                  ),
                  const SizedBox(height: 20),

                  // Firebase Data Display Section
                  Container(
                    padding: const EdgeInsets.all(16.0),
                    decoration: BoxDecoration(
                      color: Colors.white.withOpacity(0.9),
                      borderRadius: BorderRadius.circular(12),
                      boxShadow: [
                        BoxShadow(
                          color: Colors.black.withOpacity(0.2),
                          blurRadius: 10,
                          offset: const Offset(0, 5),
                        ),
                      ],
                    ),
                    child: Text(
                      viewModel.displayedText.isNotEmpty
                          ? viewModel.displayedText
                          : 'Listening for data...',
                      style: const TextStyle(
                        fontSize: 18,
                        color: Colors.black87,
                        fontWeight: FontWeight.w500,
                      ),
                      textAlign: TextAlign.center,
                    ),
                  ),

                  // Buttons Section
                  Column(
                    children: [
                      CommonButton(
                          text: 'Clear Text',
                          onpress: () {
                            viewModel.clearFirebaseData();
                          }),
                      const SizedBox(
                        height: 100,
                      ),
                      Text(
                        'User Speech: ${viewModel.userSpeech}',
                        style: const TextStyle(
                            fontSize: 24,
                            fontStyle: FontStyle.italic,
                            color: Colors.black),
                      ),
                      const SizedBox(height: 20),
                      CommonButton(
                          text: 'Clear Speech',
                          onpress: () {
                            viewModel.clearUserspeech();
                          })
                    ],
                  ),
                ],
              ),
            ),
          ),
          floatingActionButton: FloatingActionButton(
            backgroundColor: Colors.white,
            onPressed: () {
              if (viewModel.isListening) {
                viewModel.stopListening();
              } else {
                viewModel.listenToUser();
              }
            },
            child: Icon(
              viewModel.isListening ? Icons.mic_off : Icons.mic,
              color: Colors.green,
            ),
          ),
        );
      },
    );
  }
}
