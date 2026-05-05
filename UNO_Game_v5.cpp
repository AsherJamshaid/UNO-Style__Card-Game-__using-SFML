//	(console backend, pre-GUI)

#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
using namespace std;

// ============================================================
//  ENUMS
// ============================================================

enum class Color    { RED, BLUE, GREEN, YELLOW, WILD };
enum class CardType { NUMBER, SKIP, REVERSE, DRAW_TWO, WILD, WILD_DRAW_FOUR };

string colorToString(Color c)
{
    switch (c) {
        case Color::RED:    return "Red";
        case Color::BLUE:   return "Blue";
        case Color::GREEN:  return "Green";
        case Color::YELLOW: return "Yellow";
        case Color::WILD:   return "Wild";
        default:            return "Unknown";
    }
}

string cardTypeToString(CardType t)
{
    switch (t) {
        case CardType::NUMBER:         return "Number";
        case CardType::SKIP:           return "Skip";
        case CardType::REVERSE:        return "Reverse";
        case CardType::DRAW_TWO:       return "Draw Two";
        case CardType::WILD:           return "Wild";
        case CardType::WILD_DRAW_FOUR: return "Wild Draw Four";
        default:                       return "Unknown";
    }
}

// ============================================================
//  CARD  (abstract base)
// ============================================================

class Card
{
protected:
    Color    color;
    CardType type;

public:
    Card(Color color, CardType type) : color(color), type(type) {}
    virtual ~Card() = default;

    Color    getColor() const { return color; }
    CardType getType()  const { return type;  }

    virtual string getLabel() const = 0;
    virtual int    getValue() const { return -1; }
    virtual bool   operator==(const Card& other) const = 0;
};

// ============================================================
//  NORMAL CARD
// ============================================================

class NormalCard : public Card
{
    int value;
public:
    NormalCard(Color color, int v) : Card(color, CardType::NUMBER), value(v) {}

    int    getValue() const override { return value; }
    string getLabel() const override { return colorToString(color) + " " + to_string(value); }

    bool operator==(const Card& other) const override
    {
        return color == other.getColor() || value == other.getValue();
    }
};

// ============================================================
//  SPECIAL CARD
// ============================================================

class SpecialCard : public Card
{
public:
    SpecialCard(Color color, CardType type) : Card(color, type) {}

    string getLabel() const override
    {
        switch (type) {
            case CardType::SKIP:           return colorToString(color) + " Skip";
            case CardType::REVERSE:        return colorToString(color) + " Reverse";
            case CardType::DRAW_TWO:       return colorToString(color) + " Draw Two";
            case CardType::WILD:           return "Wild";
            case CardType::WILD_DRAW_FOUR: return "Wild Draw Four";
            default:                       return "Unknown Special";
        }
    }

    bool operator==(const Card& other) const override
    {
        return color == other.getColor() || type == other.getType();
    }
};

// ============================================================
//  UNO DECK  (Factory — infinite procedural generation)
//  Note: the 56-slot if-chain is intentional and readable.
//  A future refactor could replace it with a struct array like:
//    struct Slot { Color c; CardType t; int v; };
//    Slot table[56] = { {BLUE,NUMBER,0}, ... };
//  and map id directly to table[id-1]. Worth doing before GUI.
// ============================================================

class UnoDeck
{
    static bool seeded;

public:
    static Card* GenerateOneCard()
    {
        if (!seeded) { srand((unsigned)time(nullptr)); seeded = true; }
        int id = rand() % 56 + 1;

        // BLUE 0-9 → id 1-10
        if (id == 1)  return new NormalCard(Color::BLUE, 0);
        if (id == 2)  return new NormalCard(Color::BLUE, 1);
        if (id == 3)  return new NormalCard(Color::BLUE, 2);
        if (id == 4)  return new NormalCard(Color::BLUE, 3);
        if (id == 5)  return new NormalCard(Color::BLUE, 4);
        if (id == 6)  return new NormalCard(Color::BLUE, 5);
        if (id == 7)  return new NormalCard(Color::BLUE, 6);
        if (id == 8)  return new NormalCard(Color::BLUE, 7);
        if (id == 9)  return new NormalCard(Color::BLUE, 8);
        if (id == 10) return new NormalCard(Color::BLUE, 9);

        // GREEN 0-9 → id 11-20
        if (id == 11) return new NormalCard(Color::GREEN, 0);
        if (id == 12) return new NormalCard(Color::GREEN, 1);
        if (id == 13) return new NormalCard(Color::GREEN, 2);
        if (id == 14) return new NormalCard(Color::GREEN, 3);
        if (id == 15) return new NormalCard(Color::GREEN, 4);
        if (id == 16) return new NormalCard(Color::GREEN, 5);
        if (id == 17) return new NormalCard(Color::GREEN, 6);
        if (id == 18) return new NormalCard(Color::GREEN, 7);
        if (id == 19) return new NormalCard(Color::GREEN, 8);
        if (id == 20) return new NormalCard(Color::GREEN, 9);

        // YELLOW 0-9 → id 21-30
        if (id == 21) return new NormalCard(Color::YELLOW, 0);
        if (id == 22) return new NormalCard(Color::YELLOW, 1);
        if (id == 23) return new NormalCard(Color::YELLOW, 2);
        if (id == 24) return new NormalCard(Color::YELLOW, 3);
        if (id == 25) return new NormalCard(Color::YELLOW, 4);
        if (id == 26) return new NormalCard(Color::YELLOW, 5);
        if (id == 27) return new NormalCard(Color::YELLOW, 6);
        if (id == 28) return new NormalCard(Color::YELLOW, 7);
        if (id == 29) return new NormalCard(Color::YELLOW, 8);
        if (id == 30) return new NormalCard(Color::YELLOW, 9);

        // RED 0-9 → id 31-40
        if (id == 31) return new NormalCard(Color::RED, 0);
        if (id == 32) return new NormalCard(Color::RED, 1);
        if (id == 33) return new NormalCard(Color::RED, 2);
        if (id == 34) return new NormalCard(Color::RED, 3);
        if (id == 35) return new NormalCard(Color::RED, 4);
        if (id == 36) return new NormalCard(Color::RED, 5);
        if (id == 37) return new NormalCard(Color::RED, 6);
        if (id == 38) return new NormalCard(Color::RED, 7);
        if (id == 39) return new NormalCard(Color::RED, 8);
        if (id == 40) return new NormalCard(Color::RED, 9);

        // SKIP x4 → id 41-44
        if (id == 41) return new SpecialCard(Color::BLUE,   CardType::SKIP);
        if (id == 42) return new SpecialCard(Color::GREEN,  CardType::SKIP);
        if (id == 43) return new SpecialCard(Color::YELLOW, CardType::SKIP);
        if (id == 44) return new SpecialCard(Color::RED,    CardType::SKIP);

        // REVERSE x4 → id 45-48
        if (id == 45) return new SpecialCard(Color::BLUE,   CardType::REVERSE);
        if (id == 46) return new SpecialCard(Color::GREEN,  CardType::REVERSE);
        if (id == 47) return new SpecialCard(Color::YELLOW, CardType::REVERSE);
        if (id == 48) return new SpecialCard(Color::RED,    CardType::REVERSE);

        // DRAW TWO x4 → id 49-52
        if (id == 49) return new SpecialCard(Color::BLUE,   CardType::DRAW_TWO);
        if (id == 50) return new SpecialCard(Color::GREEN,  CardType::DRAW_TWO);
        if (id == 51) return new SpecialCard(Color::YELLOW, CardType::DRAW_TWO);
        if (id == 52) return new SpecialCard(Color::RED,    CardType::DRAW_TWO);

        // WILD x2 → id 53-54
        if (id <= 54) return new SpecialCard(Color::WILD, CardType::WILD);

        // WILD DRAW FOUR x2 → id 55-56
        return new SpecialCard(Color::WILD, CardType::WILD_DRAW_FOUR);
    }

    static vector<Card*> GenerateCards(int count)
    {
        vector<Card*> cards;
        for (int i = 0; i < count; i++)
            cards.push_back(GenerateOneCard());
        return cards;
    }
};
bool UnoDeck::seeded = false;

// ============================================================
//  PLAYER
// ============================================================

class Player
{
    string        name;
    vector<Card*> hand;
    bool          saidUno;

public:
    Player(const string& name) : name(name), saidUno(false) {}

    ~Player() { for (Card* c : hand) delete c; }

    // --- Getters ---
    string               getName()     const { return name;             }
    bool                 getSaidUno()  const { return saidUno;          }
    int                  getHandSize() const { return (int)hand.size(); }
    const vector<Card*>& getHand()     const { return hand;             }

    // --- Mutators ---
    void setSaidUno(bool val) { saidUno = val; }

    void addCard(Card* card) { if (card) hand.push_back(card); }

    void removeCard(Card* card)
    {
        for (int i = 0; i < (int)hand.size(); i++) {
            if (hand[i] == card) {
                hand.erase(hand.begin() + i);
                if ((int)hand.size() != 1) saidUno = false;
                return;
            }
        }
    }
};

// ============================================================
//  GAME MANAGER  (Singleton)
// ============================================================

class GameManager
{
    // --- Singleton ---
    static GameManager* instance;

    // --- State ---
    Card*          topCard;
    vector<Player*> players;
    int            currentPlayerIdx;   // 0 or 1 (2-player)
    bool           gameOver;

    GameManager()
        : topCard(nullptr), currentPlayerIdx(0), gameOver(false) {}

    // ----------------------------------------------------------------
    //  Wild_and_WildDrawFour
    //
    //  Called from applyEffect after the Wild/WD4 card is already on
    //  topCard. This function:
    //    1. Asks the current player to pick a color.
    //    2. Shows only their hand cards of that color.
    //    3. Has them play one — which becomes the new topCard,
    //       so topCard->getColor() is the chosen color for the next player.
    //
    //  No currentColor field is needed because the second played card
    //  carries the color naturally.
    //
    //  DO NOT call playCard(p, wildCard) here — the Wild is already on
    //  topCard. Calling playCard on it again would re-enter applyEffect
    //  and recurse infinitely.
    // ----------------------------------------------------------------
    void Wild_and_WildDrawFour(Card* /*card*/)
    {
        Player* p = getCurrentPlayer();

        // Step 1: pick a color
        cout << "Choose a color:\n"
             << "  0: Red\n  1: Blue\n  2: Green\n  3: Yellow\n> ";
        int colorChoice;
        cin >> colorChoice;
        if (colorChoice < 0 || colorChoice > 3) {
            cout << "Invalid. Defaulting to Red.\n";
            colorChoice = 0;
        }
        Color chosen = static_cast<Color>(colorChoice);
        cout << p->getName() << " chose " << colorToString(chosen) << "!\n";
        
        cout << "Play a " << colorToString(chosen) << " card:\n";
        const vector<Card*>& h = p->getHand();
        displaycardsinhand(h);
        int pick;
        cout << "> ";
        cin >> pick;
        if (h[pick]->getColor() != chosen) {
            cout << "That card does not match the chosen color. Playing first card of that color instead.\n";
            for (int i = 0; i < (int)h.size(); i++) {
                if (h[i]->getColor() == chosen) {
                    pick = i;
                    break;
                }
            }
        }
        playCard(p, h[pick]);
    }

    // ----------------------------------------------------------------
    //  playCard — removes card from hand, places on topCard, applies effect
    // ----------------------------------------------------------------
    void playCard(Player* p, Card* card)
    {
        p->removeCard(card);
        delete topCard;
        topCard = card;
        applyEffect(card);
    }

    // ----------------------------------------------------------------
    //  applyEffect — rule side-effects after card lands on topCard.
    //
    //  Skip/Reverse: pre-advance here so playerTurn()'s final advanceTurn
    //    double-toggles back to the same player (net = skip other player).
    //
    //  Draw Two / Wild Draw Four: other player draws, but NO advanceTurn —
    //    they draw and still get their normal turn (custom rule).
    // ----------------------------------------------------------------
    void applyEffect(Card* card)
    {
        switch (card->getType()) {

        case CardType::NUMBER:
            // No side-effect.
            break;

        case CardType::SKIP:
        case CardType::REVERSE:                     // Reverse = Skip in 2-player
            cout << "[" << card->getLabel() << "] "
                 << getOtherPlayer()->getName() << " loses their turn!\n";
            advanceTurn();                          // pre-advance trick
            break;

        case CardType::DRAW_TWO:
            cout << "[DRAW TWO] " << getOtherPlayer()->getName()
                 << " draws 2 cards!\n";
            getOtherPlayer()->addCard(UnoDeck::GenerateOneCard());
            getOtherPlayer()->addCard(UnoDeck::GenerateOneCard());
            // No advanceTurn — other player draws but keeps their turn.
            break;

        case CardType::WILD:
            cout << "[WILD] " << getCurrentPlayer()->getName()
                 << " played a Wild!\n";
            Wild_and_WildDrawFour(card);
            break;

        case CardType::WILD_DRAW_FOUR:
            cout << "[WILD DRAW FOUR] " << getOtherPlayer()->getName()
                 << " draws 4 cards!\n";
            for (int i = 0; i < 4; i++)
                getOtherPlayer()->addCard(UnoDeck::GenerateOneCard());
            // No advanceTurn — other player draws but keeps their turn.
            Wild_and_WildDrawFour(card);
            break;

        default:
            break;
        }
    }

public:
    static GameManager* getInstance()
    {
        if (!instance) instance = new GameManager();
        return instance;
    }

    ~GameManager()
    {
        delete topCard;
        for (Player* p : players) delete p;
    }

    // ----------------------------------------------------------------
    //  SETUP
    // ----------------------------------------------------------------

    void addPlayer(Player* p) { players.push_back(p); }

    // ----------------------------------------------------------------
    //  NAVIGATION
    // ----------------------------------------------------------------

    Player* getCurrentPlayer() { return players[currentPlayerIdx];        }
    Player* getOtherPlayer()   { return players[1 - currentPlayerIdx];    }
    void    advanceTurn()      { currentPlayerIdx = 1 - currentPlayerIdx; }

    // ----------------------------------------------------------------
    //  PLAYABILITY  (one specific card vs. topCard)
    // ----------------------------------------------------------------

    bool isPlayable(const Card* card) const
    {
        if (card->getType() == CardType::WILD ||
            card->getType() == CardType::WILD_DRAW_FOUR) return true;
        if (card->getColor() == topCard->getColor())      return true;
        if (card->getType() == topCard->getType()) {
            if (card->getType() == CardType::NUMBER)
                return card->getValue() == topCard->getValue();
            return true;
        }
        return false;
    }

    // ----------------------------------------------------------------
    //  DISPLAY
    // ----------------------------------------------------------------

    void displaycardsinhand(const vector<Card*>& h) const
    {
        for (int i = 0; i < (int)h.size(); i++)
            cout << "  " << i << ": " << h[i]->getLabel() << "\n";
    }

    // ----------------------------------------------------------------
    //  ONE PLAYER'S TURN
    //  Loop-based re-prompting — no recursion, no stack risk.
    // ----------------------------------------------------------------

    void playerTurn()
    {
        Player* p = getCurrentPlayer();
        bool    turnConsumed = false;

        while (!turnConsumed) {

            cout << "\n========================================\n";
            cout << p->getName() << "'s turn\n";
            cout << "Top card: " << topCard->getLabel()
                 << "  [Color: " << colorToString(topCard->getColor()) << "]\n";
            cout << "Your hand (" << p->getHandSize() << " cards):\n";
            displaycardsinhand(p->getHand());
            cout << "Enter card index to play, or -1 to draw: ";

            int choice;
            cin >> choice;

            if (choice == -1) {
                // --- Draw ---
                Card* drawn = UnoDeck::GenerateOneCard();
                p->addCard(drawn);                          // add before anything else
                cout << "Drew: " << drawn->getLabel() << "\n";

                if (isPlayable(drawn)) {
                    cout << "Drawn card is playable! Play it? (1=Yes / 0=Keep): ";
                    int play;
                    cin >> play;
                    if (play == 1)
                        playCard(p, drawn);                 // drawn now owned by topCard
                    else
                        cout << "Kept in hand. Turn passes.\n";
                } else {
                    cout << "Not playable — kept in hand. Turn passes.\n";
                }
                turnConsumed = true;

            } else if (choice >= 0 && choice < p->getHandSize()) {
                Card* selected = p->getHand()[choice];
                if (isPlayable(selected)) {
                    playCard(p, selected);
                    turnConsumed = true;
                } else {
                    cout << "That card cannot be played. Try again.\n";
                    // stays in loop
                }
            } else {
                cout << "Invalid input. Try again.\n";
                // stays in loop
            }
        }

        // --- Win check ---
        if (p->getHandSize() == 0) {
            cout << "\n*** " << p->getName() << " wins the round! ***\n";
            gameOver = true;
            return;
        }

        // --- UNO call (penalty logic to be added) ---
        if (p->getHandSize() == 1)
            cout << "[UNO] " << p->getName() << " has one card left!\n";

        advanceTurn();
    }

    // ----------------------------------------------------------------
    //  STARTUP
    // ----------------------------------------------------------------

    void startGame()
    {
        for (Player* p : players) {
            vector<Card*> dealt = UnoDeck::GenerateCards(10); // 10 for debug; 7 for release
            for (Card* c : dealt) p->addCard(c);
        }

        // First top card must not be a Wild
        do {
            delete topCard;
            topCard = UnoDeck::GenerateOneCard();
        } while (topCard->getType() == CardType::WILD ||
                 topCard->getType() == CardType::WILD_DRAW_FOUR);

//        cout << "Game started! First card: " << topCard->getLabel() << "\n";
    }

    // ----------------------------------------------------------------
    //  MAIN GAME LOOP
    // ----------------------------------------------------------------

    void runGame()
    {
        startGame();
        while (!gameOver)
            playerTurn();
    }
};
GameManager* GameManager::instance = nullptr;

// ============================================================
//  MAIN
// ============================================================

int main()
{
    GameManager* game = GameManager::getInstance();
    game->addPlayer(new Player("Bablu"));
    game->addPlayer(new Player("Ali"));
    game->runGame();
    delete game;
    return 0;
}
