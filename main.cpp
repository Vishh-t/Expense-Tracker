#include <iostream>
#include "helpers.h"
#include "auth.h"
#include "expense_tracker.h"


using namespace std;

int main()
{
    WORD old = save_color();
    setColor(CYAN);
    cout << "\n============================================\n";
    cout << "       EXPENSE TRACKER LOGIN                \n";
    cout << "============================================\n\n";
    setColor(old);

    if (!Authenticate())
    {
        return 0;
    }

    expense_tracker e;
    e.load_from_file();
    e.run();

    return 0;
}
