#include "db.h"
#include "mux.h"
#include "web-app.h"

using namespace std;
using namespace web::core;
using namespace web::application;

int main() {

    auto dbPtr = new DefaultDb("db1", "usr1", "pwd1");
    auto muxPtr = new DefaultMux();
    auto webapp = make_unique<WebApp>(dbPtr, muxPtr);
    webapp->run();
    return 0;
}