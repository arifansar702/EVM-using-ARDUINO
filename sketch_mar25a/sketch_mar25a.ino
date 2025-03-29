#include <LiquidCrystal.h>

// LCD pin connections
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// Button pins
#define S1 7  // Button for Candidate A
#define S2 6  // Button for Candidate B
#define S3 5  // Button for Candidate C
#define S4 4  // Button for Candidate D
#define S5 3  // Button to show results

// Vote counters
int vote1 = 0;
int vote2 = 0;
int vote3 = 0;
int vote4 = 0;

// Button debounce variables
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 200;

void setup() {
  // Initialize buttons as inputs with pull-up resistors
  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);
  pinMode(S5, INPUT_PULLUP);

  // Initialize LCD
  lcd.begin(16, 2);
  
  // Display welcome message
  lcd.print("  Electronic  ");
  lcd.setCursor(0, 1);
  lcd.print(" Voting Machine ");
  delay(3000);
  
  // Display candidate options
  displayCandidates();
}

void loop() {
  // Check for votes with debounce
  if (digitalRead(S1) == LOW && millis() - lastDebounceTime > debounceDelay) {
    vote1++;
    lastDebounceTime = millis();
    updateDisplay();
    while (digitalRead(S1) == LOW); // Wait for button release
  }

  if (digitalRead(S2) == LOW && millis() - lastDebounceTime > debounceDelay) {
    vote2++;
    lastDebounceTime = millis();
    updateDisplay();
    while (digitalRead(S2) == LOW);
  }

  if (digitalRead(S3) == LOW && millis() - lastDebounceTime > debounceDelay) {
    vote3++;
    lastDebounceTime = millis();
    updateDisplay();
    while (digitalRead(S3) == LOW);
  }

  if (digitalRead(S4) == LOW && millis() - lastDebounceTime > debounceDelay) {
    vote4++;
    lastDebounceTime = millis();
    updateDisplay();
    while (digitalRead(S4) == LOW);
  }

  // Check for results button press
  if (digitalRead(S5) == LOW && millis() - lastDebounceTime > debounceDelay) {
    lastDebounceTime = millis();
    while (digitalRead(S5) == LOW); // Wait for button release
    showResults();
  }
}

void displayCandidates() {
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("A");
  lcd.setCursor(5, 0);
  lcd.print("B");
  lcd.setCursor(9, 0);
  lcd.print("C");
  lcd.setCursor(13, 0);
  lcd.print("D");
  updateDisplay();
}

void updateDisplay() {
  lcd.setCursor(1, 1);
  lcd.print(vote1);
  lcd.setCursor(5, 1);
  lcd.print(vote2);
  lcd.setCursor(9, 1);
  lcd.print(vote3);
  lcd.setCursor(13, 1);
  lcd.print(vote4);
}

void showResults() {
  int totalVotes = vote1 + vote2 + vote3 + vote4;
  
  lcd.clear();
  
  if (totalVotes == 0) {
    lcd.print(" No Votes Cast ");
    delay(3000);
    displayCandidates();
    return;
  }

  // Find the maximum votes
  int maxVotes = max(max(vote1, vote2), max(vote3, vote4));
  
  // Count how many candidates have the maximum votes
  int winners = 0;
  if (vote1 == maxVotes) winners++;
  if (vote2 == maxVotes) winners++;
  if (vote3 == maxVotes) winners++;
  if (vote4 == maxVotes) winners++;

  if (winners > 1) {
    // There's a tie
    lcd.print("Tie Between: ");
    lcd.setCursor(0, 1);
    if (vote1 == maxVotes) lcd.print("A ");
    if (vote2 == maxVotes) lcd.print("B ");
    if (vote3 == maxVotes) lcd.print("C ");
    if (vote4 == maxVotes) lcd.print("D ");
  } else {
    // Single winner
    lcd.print("Winner is: ");
    lcd.setCursor(0, 1);
    if (vote1 == maxVotes) lcd.print("A");
    else if (vote2 == maxVotes) lcd.print("B");
    else if (vote3 == maxVotes) lcd.print("C");
    else if (vote4 == maxVotes) lcd.print("D");
  }
  
  delay(5000);
  
  // Show vote counts
  lcd.clear();
  lcd.print("Votes: A B C D");
  lcd.setCursor(0, 1);
  lcd.print("      ");
  lcd.print(vote1);
  lcd.print(" ");
  lcd.print(vote2);
  lcd.print(" ");
  lcd.print(vote3);
  lcd.print(" ");
  lcd.print(vote4);
  
  delay(5000);
  
  // Reset votes and return to candidate display
  vote1 = vote2 = vote3 = vote4 = 0;
  displayCandidates();
}