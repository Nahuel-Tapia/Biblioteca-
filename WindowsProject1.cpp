#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


// Definición de la estructura Cliente
typedef struct {
    char nombre[50];
    int dni;
    int edad;
} Cliente;

Cliente clientes[100];
int cantidad_reg = 0;

// Identificadores para los controles
#define ID_BUTTON_REGISTRAR 104
#define ID_BUTTON_VER_CLIENTES 105
#define ID_LISTBOX_CLIENTES 106
#define ID_DIALOGO_REGISTRO 107
#define IDC_EDIT_NOMBRE 1001
#define IDC_EDIT_DNI 1002
#define IDC_EDIT_EDAD 1003
#define IDM_EXIT 105

// Prototipos de funciones
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void MostrarClientes(HWND hwnd);

// Función principal
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Biblioteca",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BUTTON_REGISTRAR:
            DialogBox((HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), MAKEINTRESOURCE(ID_DIALOGO_REGISTRO), hwnd, DialogProc);
            break;
        case ID_BUTTON_VER_CLIENTES:
            MostrarClientes(hwnd);
            break;
        }
        break;

    case WM_CREATE: {
        // Crear botones en la ventana principal
        CreateWindow(L"BUTTON", L"Registrar Cliente", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 10, 150, 30, hwnd, (HMENU)ID_BUTTON_REGISTRAR, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        CreateWindow(L"BUTTON", L"Ver Clientes", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 50, 150, 30, hwnd, (HMENU)ID_BUTTON_VER_CLIENTES, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

        // Crear lista para mostrar clientes
        CreateWindow(L"LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | LBS_NOTIFY,
            10, 100, 360, 150, hwnd, (HMENU)ID_LISTBOX_CLIENTES, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);
        break;
    }

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            wchar_t nombre[50] = L"", dniStr[10] = L"", edadStr[10] = L"";
            int dni, edad;

            // Obtener los valores de los cuadros de texto
            GetDlgItemText(hDlg, IDC_EDIT_NOMBRE, nombre, 50);
            GetDlgItemText(hDlg, IDC_EDIT_DNI, dniStr, 10);
            GetDlgItemText(hDlg, IDC_EDIT_EDAD, edadStr, 10);

            // Convertir los valores a los tipos correctos
            dni = _wtoi(dniStr);
            edad = _wtoi(edadStr);

            // Almacenar los valores en la estructura de clientes
            Cliente nuevo_cliente;
            wcstombs_s(NULL, nuevo_cliente.nombre, sizeof(nuevo_cliente.nombre), nombre, sizeof(nuevo_cliente.nombre) - 1);
            nuevo_cliente.dni = dni;
            nuevo_cliente.edad = edad;

            clientes[cantidad_reg++] = nuevo_cliente;

            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void MostrarClientes(HWND hwnd) {
    HWND hList = GetDlgItem(hwnd, ID_LISTBOX_CLIENTES);
    SendMessage(hList, LB_RESETCONTENT, 0, 0);  // Limpiar la lista

    for (int i = 0; i < cantidad_reg; i++) {
        wchar_t buffer[100];
        swprintf(buffer, 100, L"Nombre: %hs, DNI: %d, Edad: %d", clientes[i].nombre, clientes[i].dni, clientes[i].edad);
        SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)buffer);
    }
}
