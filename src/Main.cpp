#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <chrono>
#include <ctime>
#include <iomanip>

struct Questions
{
    std::string question;
    std::string answers[3];
    int correctAnswer;
};

int askQuestion(const Questions &question)
{
    int choice;
    std::cout << question.question << '\n';
    for (size_t i = 0; i < 3; i++)
    {
        std::cout << question.answers[i] << '\n';
    }

    // Input validation
    while (true)
    {
        std::cout << "Choose 1-3: ";
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 3)
        {
            std::cin.clear(); // clears the error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a valid input(1 - 3): ";
        }
        else
        {
            break;
        }
    }

    return choice;
}

std::string validateUserAnswer(const Questions &question, int userChoice)
{
    if (userChoice == question.correctAnswer)
    {
        return "Correct!\n\n";
    }
    else
    {
        return "Incorrect\n\n";
    }
}

bool storeUserAnswersIntoAFile(const Questions &question)
{
    std::fstream userAnswersFile;
    userAnswersFile.open("answers.txt", std::ios::app);
    if (!userAnswersFile.is_open())
        return false;

    int userChoice = askQuestion(question);
    std::string result{validateUserAnswer(question, userChoice)};
    std::cout << result;

    userAnswersFile << "Question: " << question.question << '\n';
    userAnswersFile << "Your answer: " << question.answers[userChoice - 1] << '\n';
    userAnswersFile << "Result: " << result;
    userAnswersFile << "--------------------------\n";

    userAnswersFile.close();

    return userChoice == question.correctAnswer;
}

void printUserAnswers()
{
    std::fstream userAnswersFile;
    userAnswersFile.open("answers.txt", std::ios::in);
    if (userAnswersFile.is_open())
    {
        std::string answers;
        while (getline(userAnswersFile, answers))
        {
            std::cout << answers << '\n';
        }
    }
    userAnswersFile.close();
}

std::string getCurrentTimeString()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    return std::string(std::ctime(&now_time));
}

int main()
{
    int choice;

    // Question 1
    Questions question1{
        "What is the smallest country?",
        {"1. USA", "2. India", "3. Vatican city"},
        3,
    };

    Questions question2{
        "What is the biggest animal in the world?",
        {"1. Elephant", "2. Blue whale", "3. Great white shark"},
        2};

    Questions question3{
        "How many elements are there in the periodic table?",
        {"1. 118 elements", "2. 119 elements", "3. 120 elements"},
        1};

    std::ofstream clearFileContents("answers.txt", std::ios::trunc);
    clearFileContents.close();

    // Recording starting time
    std::string startTime = getCurrentTimeString();

    int correctCount{0};
    int totalQuestions{3};

    if (storeUserAnswersIntoAFile(question1))
        correctCount++;
    if (storeUserAnswersIntoAFile(question2))
        correctCount++;
    if (storeUserAnswersIntoAFile(question3))
        correctCount++;

    std::string endTime = getCurrentTimeString();

    double percentage = static_cast<double>(correctCount) / totalQuestions * 100;

    std::ofstream userAnswersFile("answers.txt", std::ios::app);
    if (userAnswersFile.is_open())
    {
        userAnswersFile << "Quiz Started at: " << startTime;
        userAnswersFile << "Quiz Ended at: " << endTime;
        userAnswersFile << "Score: " << correctCount << "/" << totalQuestions << " (" << std::fixed << std::setprecision(2) << percentage << "%)\n";
        userAnswersFile << "===============================\n";
    }
    userAnswersFile.close();

    std::cout << "\nHere are your recorded answers:\n";
    printUserAnswers();
}