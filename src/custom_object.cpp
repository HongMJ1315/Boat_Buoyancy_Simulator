#include "custom_object.h"

CustomObject::CustomObject(glm::vec3 loc, glm::vec3 v, glm::vec3 a, float m)
    : object(loc, v, a, m){
    // initializeVertices();
}

void CustomObject::initializeVertices(std::vector<glm::vec3> &vertices){
    // float tmp[] = {
    //     -10.0f, -10.0f, 0.0f,
    //      10.f, -10.f, 0.0f,
    //      0.0f,  10.f, 0.0f
    // };
    float *tmp = new float[vertices.size() * 3];
    for(int i = 0; i < vertices.size(); i++){
        tmp[i * 3] = vertices[i].x;
        tmp[i * 3 + 1] = vertices[i].y;
        tmp[i * 3 + 2] = vertices[i].z;
    }
    // 創建一個 VAO
    glGenVertexArrays(1, &vao);

    // 綁定 VAO
    glBindVertexArray(vao);

    // 創建一個 VBO
    glGenBuffers(1, &vbo);

    // 綁定 VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // 將頂點數據複製到 VBO 中
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, tmp, GL_STATIC_DRAW);

    // 設定頂點屬性指針
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // 解綁 VBO 和 VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void CustomObject::draw(){

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 3); // Assuming vertices represent triangles
    glBindVertexArray(0);
}
