
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>

 // Struct to hold each comment's data
struct CommentEntry {
    std::string comment;
    std::string sentiment;
    std::string textBlobPrediction;
    std::string vaderPrediction;
};

// Function to load CSV data into a vector
std::vector<CommentEntry> loadComments(const std::string& filename) {
    std::vector<CommentEntry> entries;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << "\n";
        return entries;
    }

    std::string line;
    getline(file, line); // skip header

    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string comment, sentiment, textBlob, vader;

        getline(ss, comment, ',');
        getline(ss, sentiment, ',');
        getline(ss, textBlob, ',');
        getline(ss, vader, ',');

        entries.push_back({ comment, sentiment, textBlob, vader });
    }

    return entries;
}

// Display all comments in short form
void displayAll(const std::vector<CommentEntry>& entries) {
    std::cout << "\nShowing All Comments:\n";
    std::cout << std::left << std::setw(6) << "No." << std::setw(80) << "Comment"
        << std::setw(12) << "Original" << std::setw(12) << "TextBlob" << std::setw(12) << "Vader" << "\n";
    std::cout << std::string(120, '-') << "\n";

    for (size_t i = 0; i < entries.size(); ++i) {
        std::string shortComment = entries[i].comment.length() > 75
            ? entries[i].comment.substr(0, 72) + "..."
            : entries[i].comment;
        std::cout << std::setw(6) << (i + 1)
            << std::setw(80) << shortComment
            << std::setw(12) << entries[i].sentiment
            << std::setw(12) << entries[i].textBlobPrediction
            << std::setw(12) << entries[i].vaderPrediction << "\n";
    }
}

// Display filtered results for top 10 matches
void displayFiltered(const std::vector<CommentEntry>& entries, const std::string& target, const std::string& source) {
    std::cout << "\nShowing Top 10 comments predicted as [" << target << "] from [" << source << "]\n";
    std::cout << std::left << std::setw(6) << "No." << std::setw(80) << "Comment"
        << std::setw(12) << "Prediction" << "\n";
    std::cout << std::string(100, '-') << "\n";

    int shown = 0;
    for (size_t i = 0; i < entries.size(); ++i) {
        std::string prediction;
        if (source == "sentiment") prediction = entries[i].sentiment;
        else if (source == "textBlob") prediction = entries[i].textBlobPrediction;
        else if (source == "vader") prediction = entries[i].vaderPrediction;

        if (prediction == target) {
            std::string display = entries[i].comment.length() > 75
                ? entries[i].comment.substr(0, 72) + "..."
                : entries[i].comment;
            std::cout << std::setw(6) << (i + 1) << std::setw(80) << display << std::setw(12) << prediction << "\n";
            shown++;
        }
        if (shown == 10) break;
    }

    if (shown == 0) std::cout << "No results matched.\n";
}

// Count all sentiments
void countSentiments(const std::vector<CommentEntry>& entries) {
    int posO = 0, negO = 0, neuO = 0;
    int posT = 0, negT = 0, neuT = 0;
    int posV = 0, negV = 0, neuV = 0;

    for (const auto& e : entries) {
        // Original
        if (e.sentiment == "positive") posO++;
        else if (e.sentiment == "negative") negO++;
        else neuO++;

        // TextBlob
        if (e.textBlobPrediction == "positive") posT++;
        else if (e.textBlobPrediction == "negative") negT++;
        else neuT++;

        // Vader
        if (e.vaderPrediction == "positive") posV++;
        else if (e.vaderPrediction == "negative") negV++;
        else neuV++;
    }

    std::cout << "\nSentiment Counts:\n";
    std::cout << std::setw(15) << "Model"
        << std::setw(10) << "Positive"
        << std::setw(10) << "Negative"
        << std::setw(10) << "Neutral" << "\n";
    std::cout << std::string(45, '-') << "\n";
    std::cout << std::setw(15) << "Original"
        << std::setw(10) << posO
        << std::setw(10) << negO
        << std::setw(10) << neuO << "\n";
    std::cout << std::setw(15) << "TextBlob"
        << std::setw(10) << posT
        << std::setw(10) << negT
        << std::setw(10) << neuT << "\n";
    std::cout << std::setw(15) << "Vader"
        << std::setw(10) << posV
        << std::setw(10) << negV
        << std::setw(10) << neuV << "\n";
}

// Show full comment by index
void showFullComment(const std::vector<CommentEntry>& entries) {
    int idx;
    std::cout << "Enter comment number (1 - " << entries.size() << "): ";
    std::cin >> idx;

    if (idx < 1 || idx > static_cast<int>(entries.size())) {
        std::cout << "Invalid number.\n";
        return;
    }

    const auto& e = entries[idx - 1];
    std::cout << "\nFull Comment #" << idx << ":\n";
    std::cout << e.comment << "\n";
    std::cout << "Original: " << e.sentiment
        << " | TextBlob: " << e.textBlobPrediction
        << " | Vader: " << e.vaderPrediction << "\n";
}

int main() {
    std::vector<CommentEntry> entries = loadComments("YTSentimentPredictions (1).csv");

    if (entries.empty()) {
        std::cout << "No data to process.\n";
        return 1;
    }

    int choice;
    do {
        std::cout << "\n================ Sentiment Analysis Menu ================\n";
        std::cout << "1. Show All Comments\n";
        std::cout << "2. Show Top 10 Positive (Original)\n";
        std::cout << "3. Show Top 10 Positive (TextBlob)\n";
        std::cout << "4. Show Top 10 Positive (Vader)\n";
        std::cout << "5. Count All Sentiments\n";
        std::cout << "6. View Full Comment by Number\n";
        std::cout << "0. Exit\n";
        std::cout << "========================================================\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::cin.ignore(); // clear newline

        switch (choice) {
        case 1: displayAll(entries); break;
        case 2: displayFiltered(entries, "positive", "sentiment"); break;
        case 3: displayFiltered(entries, "positive", "textBlob"); break;
        case 4: displayFiltered(entries, "positive", "vader"); break;
        case 5: countSentiments(entries); break;
        case 6: showFullComment(entries); break;
        case 0: std::cout << "Goodbye!\n"; break;
        default: std::cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}
