#include<gl/glut.h>
#include<math.h>
#include<vector>
using namespace std;

class vec3 {    
public:
    float m[3];
    vec3(float x = 0, float y = 0, float z = 0) {
        m[0] = x; m[1] = y; m[2] = z;
    }
    vec3 operator+(vec3 x) {
        return vec3(m[0] + x.m[0],
            m[1] + x.m[1],
            m[2] + x.m[2]);
    }
    vec3 operator-(vec3 x) {
        return vec3(m[0] - x.m[0],
            m[1] - x.m[1],
            m[2] - x.m[2]);
    }
    vec3 operator*(float x) {
        return vec3(m[0] * x, m[1] * x, m[2] * x);
    }
    float Length() {
        return sqrtf(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
    }
};

class IronBall {
public:
    vec3 v;
    vec3 pos;
};

class Tree {
public:
    vec3 pos;
    vec3 v;
};

vector<IronBall> personIronballs; //사람이 던지는 공 정보
vector<IronBall> robotIronballs;  //로봇이 던지는 공 정보

vector<Tree> trees;
int MX = 300;   //카메라 조절을 위한 변수들
int MY = 300;

float time = 0;      //로봇의 이동을 위한 시간 변수
float value = 0;    //팔, 다리 움직임을 위한 시간 변수

float x = 0, y = 0;  //사람의 좌표 
int robotLife = 20, personLife = 10; //로봇은 20번, 사람은 10번까지 공을 맞을 수 있다.
float robotThrowTime = 0;  //로봇이 공을 던지는 시간
float treeTime = 0;

float mat_floor[4] = { 0, 0.5, 0 ,1 };  //바닥 재질
float mat_person[4] = { 0.7, 0.7, 0,1 };//사람 재질
float mat_robot[4] = { 0.2,0.3, 0.7 };//로봇 재질
float mat_robot_wheel[4] = { 0.7,0.7,0.7 }; //바퀴 재질
float mat_iron[4] = { 1.0,1.0,1.0 }; //사람이 던지는 iron ball 재질
float mat_robotIron[4] = { 0.3,0.3,0.3 }; //로봇이 던지는 iron ball 재질
float mat_lifeBox[4] = { 1,0,0.5 }; //생명 박스 재질
float mat_leaf[4] = { 0,1,0 }; //나무 잎
float mat_tree[4] = { 0.7,0.3,0 }; //나무

void DrawLifeBoxRobot() { //로봇의 생명 박스 그리기
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_lifeBox); //빨강
    for (int i = 1; i <= 20; i++) {
        if (robotLife >= i) { //남은 생명만큼
            glutSolidCube(2); //solid cube를 그리고
        }
        else { //줄어든 생명만큼
            glutWireCube(2); //wire cube를 그린다.
        }
        glTranslatef(2.0, 0.0, 0.0);
    } //로봇의 최대 생명은 20이므로 20번 반복

    glPopMatrix();
}
void DrawLifeBoxPerson() {//사람의 생명 박스 그리기
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_lifeBox); //빨강
    for (int i = 1; i <= 10; i++) {
        if (personLife >= i) { //남은 생명만큼
            glutSolidCube(2); //solid cube를 그리고
        }
        else { //줄어든 생명만큼
            glutWireCube(2);  //wire cube를 그린다.
        }
        glTranslatef(2.0, 0.0, 0.0);
    } //사람의 최대 생명은 10이므로 10번 반복

    glPopMatrix();
}

void DrawPerson() {
    glPushMatrix();
        glTranslatef(0, 6.7, 0);

        glPushMatrix();
            glScalef(0.3, 1.0, 0.3);
            glutSolidSphere(4, 20, 20); //몸통
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0, 4.3, 0); //목
            glPushMatrix();
                glScalef(0.1, 1, 0.3);
                glutSolidSphere(0.5, 20, 20);
            glPopMatrix();
            glPushMatrix(); //머리
                glRotatef(sin(value * 0.005) * 30, 1, 0, 0);
                glTranslatef(0, 1.2, 0);
                glutSolidSphere(1, 20, 20);
            glPopMatrix();
            glPushMatrix(); //생명 박스
                glTranslatef(-3, 5, 0);
                glScalef(0.3, 0.3, 0.3);
                DrawLifeBoxPerson();
            glPopMatrix();
        glPopMatrix();
        
        //생명 상자그릴 때 제질 변경으로 인한 재질 재변경
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_person);

        //몸통으로 돌아옴
        glPushMatrix();     //오른쪽 팔
            glRotatef(10, 0, 0, -1);
            glTranslatef(0.5, 1.5, 0);
            glRotatef(-sin(value*0.005)*30+30, -1, 0, -1);
            glTranslatef(0, 1.5, 0);
            glPushMatrix();
                glScalef(0.1, 0.5, 0.1); //얇은팔을 위해 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, 1, 0); //팔목
            glPushMatrix();
                glTranslatef(0, 1.5, 0);
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //몸통으로 돌아옴

        glPushMatrix();     //왼쪽 팔
            glRotatef(sin(value* 0.005) * 30 + 30, -1, 0, 0);
            glTranslatef(-1, 0,0.5);
            glPushMatrix();
                glScalef(0.1, 0.5, 0.1); //얇은팔을 위해 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, -1, 0); //팔목
            glPushMatrix();
                glTranslatef(0, -1.5, 0);
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //몸통으로 돌아옴

        glPushMatrix(); //오른쪽 다리
            glRotatef(-sin(value * 0.005) * 15 + 15, 1, 0, 0);
            glTranslatef(1, -5, 0.5);
            glTranslatef(0, 2, 0);
            glPushMatrix();
                glScalef(0.1, 1, 0.1); //얇은다리을 위해 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, -1.8, 0); //발목
            glPushMatrix();
                glRotatef(-sin(value * 0.005) * 8 + 8, 1, 0, 0);
                glTranslatef(0, -1, 0);
                glScalef(0.2, 0.2, 0.3);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //몸통으로 돌아옴

        glPushMatrix(); //왼쪽 다리
            glRotatef(sin(value * 0.005) * 15 + 15, 1, 0, 0);
            glTranslatef(-1, -5, 0.5);
            glTranslatef(0, 2, 0);
            glPushMatrix();
                glScalef(0.1, 1, 0.1); //얇은다리을 위해 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, -1.8, 0); //발목
            glPushMatrix();
                glRotatef(sin(value * 0.005) * 8 + 8, 1, 0, 0);
                glTranslatef(0, -1, 0);
                glScalef(0.2, 0.2, 0.3);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}
void DrawRobot() {
    glPushMatrix();
        glTranslatef(0,5.8,0); //로봇 몸통
        glutSolidCube(3);
        
        glPushMatrix(); //머리
            glTranslatef(0, 2, 0);
            glScalef(0.7, 1, 0.7);
            glutSolidCube(2);
            
            glPushMatrix();     //생명 박스
                glTranslatef(-4, 3, 0);
                glScalef(0.2, 0.3, 0.3);
                DrawLifeBoxRobot();
             glPopMatrix();
        glPopMatrix();
        //몸통으로 돌아옴

        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_robot); //lifeBox를 그릴 때 제질변경으로 인해 다시 호출
        glPushMatrix();     //오른쪽 팔
            glTranslatef(1.1, 0, 0);
            glRotatef(-sin(value * 0.005) * 30 + 30, 1, 0, -1);
            glTranslatef(0, 2, 0);
            glPushMatrix();
                glScalef(0.1, 0.5, 0.1); //얇은팔을 위해 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, 1, 0); //팔목
            glPushMatrix();
                glTranslatef(0, 1.5, 0);
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //몸통으로 돌아옴

        glPushMatrix();     //왼쪽 팔
            glTranslatef(-1.1, 0, 0);
            glRotatef(sin(value * 0.005) * 30 + 30, 1, 0, 1);
            glTranslatef(0, 2, 0);
            glPushMatrix();
                glScalef(0.1, 0.5, 0.1); //얇은팔을 위해 
                glutSolidCube(5);
            glPopMatrix();
         
            glTranslatef(0, 1, 0); //팔목
            glPushMatrix();
                glTranslatef(0, 1.5, 0);
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //몸통으로 돌아옴

        glPushMatrix();  //하단 몸통 생성
            glTranslatef(0, -3, 0);
            glPushMatrix();
                glScalef(2, 1, 2);
                glutSolidCube(3);
            glPopMatrix();
           
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_robot_wheel);
            glPushMatrix();     //오른쪽 바퀴 1
                glTranslatef(-3.5, -1, -2);
                glRotatef(value, 1, 0, 0);
                glRotatef(90, 0, 1, 0);
                glutSolidTorus(0.6, 1, 12, 12);
            glPopMatrix();
            //하단 몸통으로 돌아옴

            glPushMatrix();     //오른쪽 바퀴 2
                glTranslatef(-3.5, -1, 2);
                glRotatef(value, 1, 0, 0);
                glRotatef(90, 0, 1, 0);
                glutSolidTorus(0.6, 1, 12, 12);
            glPopMatrix();
            //하단 몸통으로 돌아옴

            glPushMatrix();     //왼쪽 바퀴 1
                glTranslatef(3.5, -1, -2);
                glRotatef(value, 1, 0, 0);
                glRotatef(90, 0, 1, 0);
                glutSolidTorus(0.6, 1, 12, 12);
            glPopMatrix();
            //하단 몸통으로 돌아옴

            glPushMatrix();     //왼쪽 바퀴 2
                glTranslatef(3.5, -1, 2);
                glRotatef(value, 1, 0, 0);
                glRotatef(90, 0, 1, 0);
                glutSolidTorus(0.6, 1, 12, 12);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}
void UpdatePersonIronballs() {
    vec3 acc(0, -0.8, 0); // 중력가속도
    float ironTime = 0.008; //공이 움직이는 속도

    int size = personIronballs.size(); //벡터 크기 

    for (int i = 0; i < size; i++) {
        //속도 = 중력가속도*시간을 계속 더한다
        //떨어지는 공 표현, 단순 선형 운동이 아닌 물리 법칙 적용
        personIronballs[i].v = personIronballs[i].v + acc * ironTime; 
        //위치 = 속도 * 시간, x,y,z의 값을 모두 계산한다.
        personIronballs[i].pos = personIronballs[i].pos + personIronballs[i].v * ironTime;

        if (personIronballs[i].pos.m[1] < 0.5 + 0.25) { // 바닥충돌
            //바닥에 닿으면 튕기게 하기위해 공의 위치를 바닥판이 닿은 위치에 고정
            personIronballs[i].pos.m[1] = 0.75;
            //솟아 올라야하니 y값을 위쪽 방향으로 튕기게 한다.
            personIronballs[i].v.m[1] = fabs(personIronballs[i].v.m[1]);
        }
    }

    //경기장에서 벗어난 공이 있는지 검사
    for (int i = 0; i < personIronballs.size(); ) {
        if (personIronballs[i].pos.m[2] > 10 || personIronballs[i].pos.m[2] < -10)
            personIronballs.erase(personIronballs.begin() + i); //벗어나면 공을 제거
        else
            i++;
    }


}
void UpdateRobotIronballs() {
    vec3 acc(0, -0.8, 0); // 중력가속도
    float ironTime = 0.008;//공이 움직이는 속도

    int size = robotIronballs.size();//벡터 크기 
    for (int i = 0; i < size; i++) {
        //속도 = 중력가속도*시간을 계속 뺀다 (사람이 던지는 공과 반대로 운동을 하기위해)
        //떨어지는 공 표현, 단순 선형 운동이 아닌 물리 법칙 적용
        robotIronballs[i].v = robotIronballs[i].v - acc * ironTime;
        //위치 = 속도 * 시간, x,y,z의 값을 모두 계산한다.
        robotIronballs[i].pos = robotIronballs[i].pos - robotIronballs[i].v * ironTime;
        if (robotIronballs[i].pos.m[1] < 0.5 + 0.25) { // 바닥충돌
            //바닥에 닿으면 튕기게 하기위해 공의 위치를 바닥판이 닿은 위치에 고정
            robotIronballs[i].pos.m[1] = 0.75;
            //솟아 올라야하니 y값을 위쪽 방향으로 튕기게 한다. (사람과는 반대)
            robotIronballs[i].v.m[1] = -fabs(robotIronballs[i].v.m[1]);
        }
    }
    //경기장에서 벗어난 공이 있는지 검사
    for (int i = 0; i < robotIronballs.size(); ) {
        if (robotIronballs[i].pos.m[2] > 12 || robotIronballs[i].pos.m[2] < -12) {
            robotIronballs.erase(robotIronballs.begin() + i);//벗어나면 공을 제거
        }
        else
            i++;
    }


}
void DrawPersonIronballs() {
    UpdatePersonIronballs(); //쇠덩이 좌표의 update
    int size = personIronballs.size(); 
    for (int i = 0; i < size; i++) { //벡터에 저장한 값으로 그리기
        glPushMatrix();
            glTranslatef(personIronballs[i].pos.m[0], personIronballs[i].pos.m[1], personIronballs[i].pos.m[2]);
            glScalef(0.6, 0.8, 0.7); //쇠 공을 표현하기 위해 공을 살짝 찌그러트린다.
            glutSolidSphere(0.5, 20, 20);
        glPopMatrix();
    }
}
void DrawRobotIronballs() {
    UpdateRobotIronballs(); //쇠덩이 좌표의 update
    int size = robotIronballs.size();
    for (int i = 0; i < size; i++) { //벡터에 저장한 값으로 그리기
        glPushMatrix();
        glTranslatef(robotIronballs[i].pos.m[0], robotIronballs[i].pos.m[1], robotIronballs[i].pos.m[2]);
        glScalef(0.6, 0.8, 0.7); //쇠 공을 표현하기 위해 공을 살짝 찌그러트린다.
        glutSolidSphere(0.5, 20, 20);
        glPopMatrix();
    }
}
bool robotHitCheck() { // 공과 로봇의 몸통의 충돌이면 true리턴
    vec3 robotBody(-sin(time * 0.002) * 8, 3.5, -8);
    int size = personIronballs.size();
    for (int i = 0; i < size; i++) {
        vec3 diff = robotBody - personIronballs[i].pos; //공과 로봇의 차이
        if (diff.Length() < 2) { //그 차이가 2 이하이면 명중
            //이미 히트한 공이 다중 히트되는 것을 막기 위해 지운다
            personIronballs.erase(personIronballs.begin()+i); 
            return true;
        } 
    }
    return false;
}
bool personHitCheck() { //공과 사람의 몸통의 충돌이면 true리턴
    vec3 personBody(x, 5, y * 8);
    int size = robotIronballs.size();
    for (int i = 0; i < size; i++) {
        vec3 diff = personBody - robotIronballs[i].pos;  //공과 사람의 차이
        if (diff.Length() < 2.0) { //그 차이가 2 이하면 명중 (사람의 명중 조건을 완화, 난이도를 위해)
            //이미 히트한 공이 다중 히트 되는 것을 막기 위해 지운다.
            robotIronballs.erase(robotIronballs.begin() + i);
            return true;
        }
    }
    return false;

}

void UpdateTrees() {
    float treeTime = 0.008;//공이 움직이는 속도

    int size = trees.size();//벡터 크기 
    for (int i = 0; i < size; i++) {
        //위치 = 속도 * 시간, x,y,z의 값을 모두 계산한다.
        trees[i].pos = trees[i].pos - trees[i].v * treeTime;
    }
    //경기장에서 벗어난 나무가 있는지 검사
    for (int i = 0; i < trees.size(); ) {
        if (trees[i].pos.m[2] > 14 || trees[i].pos.m[2] < -14) {
            trees.erase(trees.begin() + i);//벗어나면 나무를 제거
        }
        else
            i++;
    }
}

void DrawTrees() { //나무 그리기
    UpdateTrees(); //나무 좌표 업데이트
    int size = trees.size();
    for (int i = 0; i < size; i++) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_tree); //갈색
        glPushMatrix(); //나무 기둥
            glTranslatef(trees[i].pos.m[0], trees[i].pos.m[1], trees[i].pos.m[2]);
            glPushMatrix();
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
            glPushMatrix();
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_leaf); //녹색
                glRotatef(90, -1, 0, 0); //나뭇잎
                glTranslatef(0, 0, 0.8);
                glScalef(0.3, 0.3, 0.6);
                glutSolidCone(3, 3, 12, 12);
            glPopMatrix();
        glPopMatrix();
    }
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_floor); //초록
    glPushMatrix(); //바닥 판
    glScalef(30, 0.5, 90); //넓고 길게 만들어 이동하는 것처럼 보이게 한다. (일종의 착시?)
    glutSolidCube(1.0);
    glPopMatrix();

    //사람
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_person);
    glPushMatrix();
    glTranslatef(0, 0, 8);
    glTranslatef(x, 0, y);
    glScalef(0.6, 0.6, 0.6);
    DrawPerson();
    glPopMatrix();

    //로봇
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_robot);
    glPushMatrix();
    glTranslatef(0, 0, -8);
    glTranslatef(-sin(time * 0.002) * 8, 0, 0); //로봇은 자동으로 좌우 8m 이동
    glScalef(0.8, 0.8, 0.8);
    DrawRobot();
    glPopMatrix();


    //쇠동이들
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_iron);
    DrawPersonIronballs();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_robotIron);
    DrawRobotIronballs();

    //나무 들
    glPushMatrix();
        glScalef(1.5, 2, 1.5);
        DrawTrees();
    glPopMatrix();

    glFlush();

    bool robotHit = robotHitCheck(); //로봇이 공에 맞았는지 검사
    if (robotHit) { //맞았으면
        robotLife -= 1;
    }
    bool personHit = personHitCheck(); //사람이 공에 맞았는지 검사
    if (personHit) { //맞았으면
        personLife -= 1;
    }

    //두 물체 중 1개의 물체라도 life가 0이하이면 게임 종료
    if (robotLife <= 0) exit(0);
    if (personLife <= 0)exit(0);
}

void MyReshape(int w,int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-15.0, 15.0, -15.0, 15.0, -100.0, 100.0); // 15가 좋은 view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(MX / 300.0, MY / 300.0, 1.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);  //시점변환
}

void MyMouseMove(GLint X, GLint Y) {
    MX = X;
    MY = Y;
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(MX/300.0,MY/300.0,1.0,
        0.0,0.0,0.0,
        0.0,1.0,0.0);
    glutPostRedisplay();
}

void MySpecial(int key, int X, int Y) {
    if (key == GLUT_KEY_UP) {
        if (y < 0.8f) {
            y += 0.5f;
        }
    }
    if (key == GLUT_KEY_DOWN) {
        if (y > -10.0f) {
            y -= 0.5f;
        }
    }
    if (key == GLUT_KEY_LEFT) {
        if (x > -11.0f) {
            x -= 0.5f;
        }
    }
    if (key == GLUT_KEY_RIGHT) {
        if (x < 11.0f) {
            x += 0.5f;
        }
    }
    glutPostRedisplay();
}

void MyKeyboard(unsigned char KeyPressed, int X, int Y) {
    if (KeyPressed == ' ') { //사람은 한손으로 공을 던진다.
        IronBall ib;
        ib.pos.m[0] = x + 1.2;
        ib.pos.m[1] = 8;
        ib.pos.m[2] = y + 8;
        ib.v.m[0] = 0;
        ib.v.m[1] = 0;
        ib.v.m[2] = -1.5;
        personIronballs.push_back(ib);
        glutPostRedisplay();
    }
}
void insertTree() { //벡터에 나무좌표, 속도 삽입
    Tree t1, t2;
    t1.pos.m[0] = -9; //왼쪽 나무
    t1.pos.m[1] = 1.5;
    t1.pos.m[2] = -9;
    t1.v.m[0] = 0;
    t1.v.m[1] = 0;
    t1.v.m[2] = -3;

    t2.pos.m[0] = 9;  //오른쪽 나무
    t2.pos.m[1] = 1.5;
    t2.pos.m[2] = -9;
    t2.v.m[0] = 0;
    t2.v.m[1] = 0;
    t2.v.m[2] = -3;

    trees.push_back(t1);
    trees.push_back(t2);
}
void insertRobotBall() { //로봇은 2개의 팔로 공을 던진다.
    IronBall ib1,ib2;
    ib1.pos.m[0] = (- sin(time * 0.002) * 8)+2; //왼손에서 던지는 공
    ib1.pos.m[1] = 8.2;
    ib1.pos.m[2] = -8;
    ib1.v.m[0] = 0;
    ib1.v.m[1] = 0;
    ib1.v.m[2] = -1.5;
    ib2.pos.m[0]= (-sin(time * 0.002) * 8)-2; //오른손에서 던지는 공
    ib2.pos.m[1] = 8.2;
    ib2.pos.m[2] = -8;
    ib2.v.m[0] = 0;
    ib2.v.m[1] = 0;
    ib2.v.m[2] = -1.5;
    robotIronballs.push_back(ib1);
    robotIronballs.push_back(ib2);
}

void MyTimer(int Value) {
    time += 1.0;    //물체의 팔, 다리를 흔들기 위한 시간 변수
    value += 2.0;   //로봇의 이동 시간 변수
    robotThrowTime += 0.005f; //로봇이 공을 생성하는 시간
    treeTime += 0.005; //나무를 생성하는 시간
    if (robotThrowTime >= 5.0f) { //5보다 크게 되면
        insertRobotBall(); //로봇은 공을 장전
        robotThrowTime = 0; //시간은 0으로 초기화 
    }
    if (treeTime >= 2.0) { //2보다 크게되면
        insertTree(); //나무를 벡터에 삽입
        treeTime = 0; //시간 0으로 초기화
    }

    glutPostRedisplay();
    glutTimerFunc(1, MyTimer, 1); //타이머 재설정
}

void InitLight() {
    GLfloat mat_diffuse[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_ambient[] = { 0.5, 0.4, 0.3, 1.0 };
    GLfloat mat_shininess[] = { 15.0 };
    GLfloat light_specular[] = { 1.0, 0.8, 1.0, 1.0 };
    GLfloat light_diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat light_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
    GLfloat light_position[] = { -3, 6, 3.0, 0.0 };
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_NORMALIZE); //조명 계산할 때, 법선 벡터를 정확히 다시 계산
}

int main(int argc,char** argv) {
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
    glutInitWindowPosition(0, 0);

	glutCreateWindow("Person and Robot Game");

	glutDisplayFunc(MyDisplay);
    glutReshapeFunc(MyReshape);
    glutKeyboardFunc(MyKeyboard);
    glutMotionFunc(MyMouseMove);
    glutSpecialFunc(MySpecial);
    glutTimerFunc(1,MyTimer,1);

    glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0.8, 0.8, 1);
	
    InitLight();
	glutMainLoop();
	return 0;
}