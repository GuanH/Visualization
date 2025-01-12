#include "App.h"

signed main(){
    App app;
    app.Run();
    glm::mat3 m = glm::mat3(1.0f);
    glm::vec3 v = m * glm::vec3(1.0f,1.0f,1.0f);
    return 0;
}
