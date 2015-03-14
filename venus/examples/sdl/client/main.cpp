#define WIN32_LEAN_AND_MEAN
//#define VENUS_CONNECTION_VENUSONLY
#define CERL_VERBOSE

#include <async/Application.h>
#include "../sdl_userfunc.h"


void cerl_callback testClient(void* lpParam)
{
	cerl::FiberParam p(lpParam);

	cerl::ScopedAlloc alloc(CerlBlockPool(p.self));

	cerl::Connection conn = cerl::connect("127.0.0.1", 8633);
	userfunc::Calc calc(conn);
	int a, b, c;
	printf("\ntest the calc function\n");
	while (1)
	{
		printf("\nselect the function u want to test:\n");
		printf("0.Quit\t1.square\t2.add\t3.dec\n4.helloworld1\t5.helloworld2\t6.async_stop\n7.square2\t8.async1\t9.async2\n>");
		scanf("%d", &a);
		switch (a)
		{
		case 0:
			{
				p.service->quit();
				exit(0);
			}
		case 1:
			{
				printf("Input an integer\n>");
				scanf("%d", &a);
				userfunc::Calc::_SquareResult result;
				calc.square(alloc, result, a);
				if (cerl::code_ok == result)
				{
					printf("square Result: '%d'!\n", result.str);
				}
			}
			break;
		case 2:
			{
				printf("Input two integer\n>");
				scanf("%d %d", &a, &b);
				userfunc::Calc::_AddResult result;
				calc.add(alloc, result, a, b);
				if (cerl::code_ok == result)
				{
					printf("add Result: '%d'!\n", result.value);
				}
				else
				{
					printf("add Operation Error with code '0x%08X'!\n", result._code);
				}
			}
			break;
		case 3:
			{
				printf("Input three integer\n>");
				scanf("%d %d %d", &a, &b, &c);
				userfunc::Calc::_DecResult result;
				calc.dec(alloc, result, a, b, c);
				if (cerl::code_ok == result)
				{
					printf("dec Result: '%d'!\n", result.value);
				}
				else
				{
					printf("dec Operation Error with code '0x%08X'!\n", result._code);
				}
			}
			break;
		case 4:
			{
				userfunc::Calc::_Helloworld1Result result;
				calc.helloworld1(alloc, result);
				if (cerl::code_ok == result)
				{
					printf("helloworld1 Result: '%d'!\n", result.value);
				}
				else
				{
					printf("Operation Error with code '0x%08X'!\n", result._code);
				}
			}
			break;
		case 5:
			{
				userfunc::Calc::_Helloworld2Result result;
				calc.helloworld2(alloc, result);
				if (cerl::code_ok == result)
				{
					printf("helloworld2 return ok\n");
				}
				else
				{
					printf("Operation Error with code '0x%08X'!\n", result._code);
				}
			}
			break;
		case 6:
			{
				calc.stop();
				printf("the async function stop is called\n");
			}
			break;
		case 7:
			{
				printf("Input an integer\n>");
				scanf("%d", &a);
				userfunc::Calc::_Square2Result result;
				calc.square2(alloc, result, a);
				if (cerl::code_ok == result)
				{
					printf("square Result: '%d'!\n", result.str);
				}
				else
				{
					if (userfunc::code_confliction == result.error.reason)
					{
						printf("code_confliction\n");
					}
					else if (userfunc::code_notimpl == result.error.reason)
					{
						printf("code_notimpl\n");
					}
				}
			}
			break;
		case 8:
			{
				printf("Input an integer\n>");
				scanf("%d", &a);
				calc.async1(a);
				printf("the async function async1 is called\n");
			}
			break;
		case 9:
			{
				printf("Input two integer\n>");
				scanf("%d %d", &a, &b);
				calc.async2(a, b);
				printf("the async function async2 is called\n");
			}
			break;
		default:
			break;
		}
	}
	p.service->quit();
}

int main()
{
	cerl::Application app;
	app.run(testClient);
	return 0;
}
