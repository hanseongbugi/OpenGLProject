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

vector<IronBall> personIronballs; //����� ������ �� ����
vector<IronBall> robotIronballs;  //�κ��� ������ �� ����

vector<Tree> trees;
int MX = 300;   //ī�޶� ������ ���� ������
int MY = 300;

float time = 0;      //�κ��� �̵��� ���� �ð� ����
float value = 0;    //��, �ٸ� �������� ���� �ð� ����

float x = 0, y = 0;  //����� ��ǥ 
int robotLife = 20, personLife = 10; //�κ��� 20��, ����� 10������ ���� ���� �� �ִ�.
float robotThrowTime = 0;  //�κ��� ���� ������ �ð�
float treeTime = 0;

float mat_floor[4] = { 0, 0.5, 0 ,1 };  //�ٴ� ����
float mat_person[4] = { 0.7, 0.7, 0,1 };//��� ����
float mat_robot[4] = { 0.2,0.3, 0.7 };//�κ� ����
float mat_robot_wheel[4] = { 0.7,0.7,0.7 }; //���� ����
float mat_iron[4] = { 1.0,1.0,1.0 }; //����� ������ iron ball ����
float mat_robotIron[4] = { 0.3,0.3,0.3 }; //�κ��� ������ iron ball ����
float mat_lifeBox[4] = { 1,0,0.5 }; //���� �ڽ� ����
float mat_leaf[4] = { 0,1,0 }; //���� ��
float mat_tree[4] = { 0.7,0.3,0 }; //����

void DrawLifeBoxRobot() { //�κ��� ���� �ڽ� �׸���
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_lifeBox); //����
    for (int i = 1; i <= 20; i++) {
        if (robotLife >= i) { //���� ����ŭ
            glutSolidCube(2); //solid cube�� �׸���
        }
        else { //�پ�� ����ŭ
            glutWireCube(2); //wire cube�� �׸���.
        }
        glTranslatef(2.0, 0.0, 0.0);
    } //�κ��� �ִ� ������ 20�̹Ƿ� 20�� �ݺ�

    glPopMatrix();
}
void DrawLifeBoxPerson() {//����� ���� �ڽ� �׸���
    glPushMatrix();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_lifeBox); //����
    for (int i = 1; i <= 10; i++) {
        if (personLife >= i) { //���� ����ŭ
            glutSolidCube(2); //solid cube�� �׸���
        }
        else { //�پ�� ����ŭ
            glutWireCube(2);  //wire cube�� �׸���.
        }
        glTranslatef(2.0, 0.0, 0.0);
    } //����� �ִ� ������ 10�̹Ƿ� 10�� �ݺ�

    glPopMatrix();
}

void DrawPerson() {
    glPushMatrix();
        glTranslatef(0, 6.7, 0);

        glPushMatrix();
            glScalef(0.3, 1.0, 0.3);
            glutSolidSphere(4, 20, 20); //����
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0, 4.3, 0); //��
            glPushMatrix();
                glScalef(0.1, 1, 0.3);
                glutSolidSphere(0.5, 20, 20);
            glPopMatrix();
            glPushMatrix(); //�Ӹ�
                glRotatef(sin(value * 0.005) * 30, 1, 0, 0);
                glTranslatef(0, 1.2, 0);
                glutSolidSphere(1, 20, 20);
            glPopMatrix();
            glPushMatrix(); //���� �ڽ�
                glTranslatef(-3, 5, 0);
                glScalef(0.3, 0.3, 0.3);
                DrawLifeBoxPerson();
            glPopMatrix();
        glPopMatrix();
        
        //���� ���ڱ׸� �� ���� �������� ���� ���� �纯��
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_person);

        //�������� ���ƿ�
        glPushMatrix();     //������ ��
            glRotatef(10, 0, 0, -1);
            glTranslatef(0.5, 1.5, 0);
            glRotatef(-sin(value*0.005)*30+30, -1, 0, -1);
            glTranslatef(0, 1.5, 0);
            glPushMatrix();
                glScalef(0.1, 0.5, 0.1); //�������� ���� 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, 1, 0); //�ȸ�
            glPushMatrix();
                glTranslatef(0, 1.5, 0);
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //�������� ���ƿ�

        glPushMatrix();     //���� ��
            glRotatef(sin(value* 0.005) * 30 + 30, -1, 0, 0);
            glTranslatef(-1, 0,0.5);
            glPushMatrix();
                glScalef(0.1, 0.5, 0.1); //�������� ���� 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, -1, 0); //�ȸ�
            glPushMatrix();
                glTranslatef(0, -1.5, 0);
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //�������� ���ƿ�

        glPushMatrix(); //������ �ٸ�
            glRotatef(-sin(value * 0.005) * 15 + 15, 1, 0, 0);
            glTranslatef(1, -5, 0.5);
            glTranslatef(0, 2, 0);
            glPushMatrix();
                glScalef(0.1, 1, 0.1); //�����ٸ��� ���� 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, -1.8, 0); //�߸�
            glPushMatrix();
                glRotatef(-sin(value * 0.005) * 8 + 8, 1, 0, 0);
                glTranslatef(0, -1, 0);
                glScalef(0.2, 0.2, 0.3);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //�������� ���ƿ�

        glPushMatrix(); //���� �ٸ�
            glRotatef(sin(value * 0.005) * 15 + 15, 1, 0, 0);
            glTranslatef(-1, -5, 0.5);
            glTranslatef(0, 2, 0);
            glPushMatrix();
                glScalef(0.1, 1, 0.1); //�����ٸ��� ���� 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, -1.8, 0); //�߸�
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
        glTranslatef(0,5.8,0); //�κ� ����
        glutSolidCube(3);
        
        glPushMatrix(); //�Ӹ�
            glTranslatef(0, 2, 0);
            glScalef(0.7, 1, 0.7);
            glutSolidCube(2);
            
            glPushMatrix();     //���� �ڽ�
                glTranslatef(-4, 3, 0);
                glScalef(0.2, 0.3, 0.3);
                DrawLifeBoxRobot();
             glPopMatrix();
        glPopMatrix();
        //�������� ���ƿ�

        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_robot); //lifeBox�� �׸� �� ������������ ���� �ٽ� ȣ��
        glPushMatrix();     //������ ��
            glTranslatef(1.1, 0, 0);
            glRotatef(-sin(value * 0.005) * 30 + 30, 1, 0, -1);
            glTranslatef(0, 2, 0);
            glPushMatrix();
                glScalef(0.1, 0.5, 0.1); //�������� ���� 
                glutSolidCube(5);
            glPopMatrix();

            glTranslatef(0, 1, 0); //�ȸ�
            glPushMatrix();
                glTranslatef(0, 1.5, 0);
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //�������� ���ƿ�

        glPushMatrix();     //���� ��
            glTranslatef(-1.1, 0, 0);
            glRotatef(sin(value * 0.005) * 30 + 30, 1, 0, 1);
            glTranslatef(0, 2, 0);
            glPushMatrix();
                glScalef(0.1, 0.5, 0.1); //�������� ���� 
                glutSolidCube(5);
            glPopMatrix();
         
            glTranslatef(0, 1, 0); //�ȸ�
            glPushMatrix();
                glTranslatef(0, 1.5, 0);
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
        glPopMatrix();
        //�������� ���ƿ�

        glPushMatrix();  //�ϴ� ���� ����
            glTranslatef(0, -3, 0);
            glPushMatrix();
                glScalef(2, 1, 2);
                glutSolidCube(3);
            glPopMatrix();
           
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_robot_wheel);
            glPushMatrix();     //������ ���� 1
                glTranslatef(-3.5, -1, -2);
                glRotatef(value, 1, 0, 0);
                glRotatef(90, 0, 1, 0);
                glutSolidTorus(0.6, 1, 12, 12);
            glPopMatrix();
            //�ϴ� �������� ���ƿ�

            glPushMatrix();     //������ ���� 2
                glTranslatef(-3.5, -1, 2);
                glRotatef(value, 1, 0, 0);
                glRotatef(90, 0, 1, 0);
                glutSolidTorus(0.6, 1, 12, 12);
            glPopMatrix();
            //�ϴ� �������� ���ƿ�

            glPushMatrix();     //���� ���� 1
                glTranslatef(3.5, -1, -2);
                glRotatef(value, 1, 0, 0);
                glRotatef(90, 0, 1, 0);
                glutSolidTorus(0.6, 1, 12, 12);
            glPopMatrix();
            //�ϴ� �������� ���ƿ�

            glPushMatrix();     //���� ���� 2
                glTranslatef(3.5, -1, 2);
                glRotatef(value, 1, 0, 0);
                glRotatef(90, 0, 1, 0);
                glutSolidTorus(0.6, 1, 12, 12);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
}
void UpdatePersonIronballs() {
    vec3 acc(0, -0.8, 0); // �߷°��ӵ�
    float ironTime = 0.008; //���� �����̴� �ӵ�

    int size = personIronballs.size(); //���� ũ�� 

    for (int i = 0; i < size; i++) {
        //�ӵ� = �߷°��ӵ�*�ð��� ��� ���Ѵ�
        //�������� �� ǥ��, �ܼ� ���� ��� �ƴ� ���� ��Ģ ����
        personIronballs[i].v = personIronballs[i].v + acc * ironTime; 
        //��ġ = �ӵ� * �ð�, x,y,z�� ���� ��� ����Ѵ�.
        personIronballs[i].pos = personIronballs[i].pos + personIronballs[i].v * ironTime;

        if (personIronballs[i].pos.m[1] < 0.5 + 0.25) { // �ٴ��浹
            //�ٴڿ� ������ ƨ��� �ϱ����� ���� ��ġ�� �ٴ����� ���� ��ġ�� ����
            personIronballs[i].pos.m[1] = 0.75;
            //�ھ� �ö���ϴ� y���� ���� �������� ƨ��� �Ѵ�.
            personIronballs[i].v.m[1] = fabs(personIronballs[i].v.m[1]);
        }
    }

    //����忡�� ��� ���� �ִ��� �˻�
    for (int i = 0; i < personIronballs.size(); ) {
        if (personIronballs[i].pos.m[2] > 10 || personIronballs[i].pos.m[2] < -10)
            personIronballs.erase(personIronballs.begin() + i); //����� ���� ����
        else
            i++;
    }


}
void UpdateRobotIronballs() {
    vec3 acc(0, -0.8, 0); // �߷°��ӵ�
    float ironTime = 0.008;//���� �����̴� �ӵ�

    int size = robotIronballs.size();//���� ũ�� 
    for (int i = 0; i < size; i++) {
        //�ӵ� = �߷°��ӵ�*�ð��� ��� ���� (����� ������ ���� �ݴ�� ��� �ϱ�����)
        //�������� �� ǥ��, �ܼ� ���� ��� �ƴ� ���� ��Ģ ����
        robotIronballs[i].v = robotIronballs[i].v - acc * ironTime;
        //��ġ = �ӵ� * �ð�, x,y,z�� ���� ��� ����Ѵ�.
        robotIronballs[i].pos = robotIronballs[i].pos - robotIronballs[i].v * ironTime;
        if (robotIronballs[i].pos.m[1] < 0.5 + 0.25) { // �ٴ��浹
            //�ٴڿ� ������ ƨ��� �ϱ����� ���� ��ġ�� �ٴ����� ���� ��ġ�� ����
            robotIronballs[i].pos.m[1] = 0.75;
            //�ھ� �ö���ϴ� y���� ���� �������� ƨ��� �Ѵ�. (������� �ݴ�)
            robotIronballs[i].v.m[1] = -fabs(robotIronballs[i].v.m[1]);
        }
    }
    //����忡�� ��� ���� �ִ��� �˻�
    for (int i = 0; i < robotIronballs.size(); ) {
        if (robotIronballs[i].pos.m[2] > 12 || robotIronballs[i].pos.m[2] < -12) {
            robotIronballs.erase(robotIronballs.begin() + i);//����� ���� ����
        }
        else
            i++;
    }


}
void DrawPersonIronballs() {
    UpdatePersonIronballs(); //�赢�� ��ǥ�� update
    int size = personIronballs.size(); 
    for (int i = 0; i < size; i++) { //���Ϳ� ������ ������ �׸���
        glPushMatrix();
            glTranslatef(personIronballs[i].pos.m[0], personIronballs[i].pos.m[1], personIronballs[i].pos.m[2]);
            glScalef(0.6, 0.8, 0.7); //�� ���� ǥ���ϱ� ���� ���� ��¦ ��׷�Ʈ����.
            glutSolidSphere(0.5, 20, 20);
        glPopMatrix();
    }
}
void DrawRobotIronballs() {
    UpdateRobotIronballs(); //�赢�� ��ǥ�� update
    int size = robotIronballs.size();
    for (int i = 0; i < size; i++) { //���Ϳ� ������ ������ �׸���
        glPushMatrix();
        glTranslatef(robotIronballs[i].pos.m[0], robotIronballs[i].pos.m[1], robotIronballs[i].pos.m[2]);
        glScalef(0.6, 0.8, 0.7); //�� ���� ǥ���ϱ� ���� ���� ��¦ ��׷�Ʈ����.
        glutSolidSphere(0.5, 20, 20);
        glPopMatrix();
    }
}
bool robotHitCheck() { // ���� �κ��� ������ �浹�̸� true����
    vec3 robotBody(-sin(time * 0.002) * 8, 3.5, -8);
    int size = personIronballs.size();
    for (int i = 0; i < size; i++) {
        vec3 diff = robotBody - personIronballs[i].pos; //���� �κ��� ����
        if (diff.Length() < 2) { //�� ���̰� 2 �����̸� ����
            //�̹� ��Ʈ�� ���� ���� ��Ʈ�Ǵ� ���� ���� ���� �����
            personIronballs.erase(personIronballs.begin()+i); 
            return true;
        } 
    }
    return false;
}
bool personHitCheck() { //���� ����� ������ �浹�̸� true����
    vec3 personBody(x, 5, y * 8);
    int size = robotIronballs.size();
    for (int i = 0; i < size; i++) {
        vec3 diff = personBody - robotIronballs[i].pos;  //���� ����� ����
        if (diff.Length() < 2.0) { //�� ���̰� 2 ���ϸ� ���� (����� ���� ������ ��ȭ, ���̵��� ����)
            //�̹� ��Ʈ�� ���� ���� ��Ʈ �Ǵ� ���� ���� ���� �����.
            robotIronballs.erase(robotIronballs.begin() + i);
            return true;
        }
    }
    return false;

}

void UpdateTrees() {
    float treeTime = 0.008;//���� �����̴� �ӵ�

    int size = trees.size();//���� ũ�� 
    for (int i = 0; i < size; i++) {
        //��ġ = �ӵ� * �ð�, x,y,z�� ���� ��� ����Ѵ�.
        trees[i].pos = trees[i].pos - trees[i].v * treeTime;
    }
    //����忡�� ��� ������ �ִ��� �˻�
    for (int i = 0; i < trees.size(); ) {
        if (trees[i].pos.m[2] > 14 || trees[i].pos.m[2] < -14) {
            trees.erase(trees.begin() + i);//����� ������ ����
        }
        else
            i++;
    }
}

void DrawTrees() { //���� �׸���
    UpdateTrees(); //���� ��ǥ ������Ʈ
    int size = trees.size();
    for (int i = 0; i < size; i++) {
        glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_tree); //����
        glPushMatrix(); //���� ���
            glTranslatef(trees[i].pos.m[0], trees[i].pos.m[1], trees[i].pos.m[2]);
            glPushMatrix();
                glScalef(0.1, 1, 0.1);
                glutSolidCube(3);
            glPopMatrix();
            glPushMatrix();
                glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_leaf); //���
                glRotatef(90, -1, 0, 0); //������
                glTranslatef(0, 0, 0.8);
                glScalef(0.3, 0.3, 0.6);
                glutSolidCone(3, 3, 12, 12);
            glPopMatrix();
        glPopMatrix();
    }
}

void MyDisplay() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_floor); //�ʷ�
    glPushMatrix(); //�ٴ� ��
    glScalef(30, 0.5, 90); //�а� ��� ����� �̵��ϴ� ��ó�� ���̰� �Ѵ�. (������ ����?)
    glutSolidCube(1.0);
    glPopMatrix();

    //���
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_person);
    glPushMatrix();
    glTranslatef(0, 0, 8);
    glTranslatef(x, 0, y);
    glScalef(0.6, 0.6, 0.6);
    DrawPerson();
    glPopMatrix();

    //�κ�
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_robot);
    glPushMatrix();
    glTranslatef(0, 0, -8);
    glTranslatef(-sin(time * 0.002) * 8, 0, 0); //�κ��� �ڵ����� �¿� 8m �̵�
    glScalef(0.8, 0.8, 0.8);
    DrawRobot();
    glPopMatrix();


    //�赿�̵�
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_iron);
    DrawPersonIronballs();
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_robotIron);
    DrawRobotIronballs();

    //���� ��
    glPushMatrix();
        glScalef(1.5, 2, 1.5);
        DrawTrees();
    glPopMatrix();

    glFlush();

    bool robotHit = robotHitCheck(); //�κ��� ���� �¾Ҵ��� �˻�
    if (robotHit) { //�¾�����
        robotLife -= 1;
    }
    bool personHit = personHitCheck(); //����� ���� �¾Ҵ��� �˻�
    if (personHit) { //�¾�����
        personLife -= 1;
    }

    //�� ��ü �� 1���� ��ü�� life�� 0�����̸� ���� ����
    if (robotLife <= 0) exit(0);
    if (personLife <= 0)exit(0);
}

void MyReshape(int w,int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-15.0, 15.0, -15.0, 15.0, -100.0, 100.0); // 15�� ���� view
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(MX / 300.0, MY / 300.0, 1.0,
        0.0, 0.0, 0.0,
        0.0, 1.0, 0.0);  //������ȯ
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
    if (KeyPressed == ' ') { //����� �Ѽ����� ���� ������.
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
void insertTree() { //���Ϳ� ������ǥ, �ӵ� ����
    Tree t1, t2;
    t1.pos.m[0] = -9; //���� ����
    t1.pos.m[1] = 1.5;
    t1.pos.m[2] = -9;
    t1.v.m[0] = 0;
    t1.v.m[1] = 0;
    t1.v.m[2] = -3;

    t2.pos.m[0] = 9;  //������ ����
    t2.pos.m[1] = 1.5;
    t2.pos.m[2] = -9;
    t2.v.m[0] = 0;
    t2.v.m[1] = 0;
    t2.v.m[2] = -3;

    trees.push_back(t1);
    trees.push_back(t2);
}
void insertRobotBall() { //�κ��� 2���� �ȷ� ���� ������.
    IronBall ib1,ib2;
    ib1.pos.m[0] = (- sin(time * 0.002) * 8)+2; //�޼տ��� ������ ��
    ib1.pos.m[1] = 8.2;
    ib1.pos.m[2] = -8;
    ib1.v.m[0] = 0;
    ib1.v.m[1] = 0;
    ib1.v.m[2] = -1.5;
    ib2.pos.m[0]= (-sin(time * 0.002) * 8)-2; //�����տ��� ������ ��
    ib2.pos.m[1] = 8.2;
    ib2.pos.m[2] = -8;
    ib2.v.m[0] = 0;
    ib2.v.m[1] = 0;
    ib2.v.m[2] = -1.5;
    robotIronballs.push_back(ib1);
    robotIronballs.push_back(ib2);
}

void MyTimer(int Value) {
    time += 1.0;    //��ü�� ��, �ٸ��� ���� ���� �ð� ����
    value += 2.0;   //�κ��� �̵� �ð� ����
    robotThrowTime += 0.005f; //�κ��� ���� �����ϴ� �ð�
    treeTime += 0.005; //������ �����ϴ� �ð�
    if (robotThrowTime >= 5.0f) { //5���� ũ�� �Ǹ�
        insertRobotBall(); //�κ��� ���� ����
        robotThrowTime = 0; //�ð��� 0���� �ʱ�ȭ 
    }
    if (treeTime >= 2.0) { //2���� ũ�ԵǸ�
        insertTree(); //������ ���Ϳ� ����
        treeTime = 0; //�ð� 0���� �ʱ�ȭ
    }

    glutPostRedisplay();
    glutTimerFunc(1, MyTimer, 1); //Ÿ�̸� �缳��
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
    glEnable(GL_NORMALIZE); //���� ����� ��, ���� ���͸� ��Ȯ�� �ٽ� ���
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