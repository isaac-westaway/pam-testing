#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <security/pam_modules.h>

static int conv_function(int num_msg, const struct pam_message **msg, struct pam_response **resp,
			void *appdata_ptr)
{
	printf("pam conv function\n");

	struct pam_response *response = (struct pam_response *)malloc(num_msg *
									sizeof(struct pam_response));

	for (int i = 0; i < num_msg; i++)
	{
		response[i].resp = strdup((const char *)appdata_ptr);
		response[i].resp_retcode = 0;
	}

	*resp = response;

	return PAM_SUCCESS;
}

int login(const char *username, const char *password)
{
	int retval;

	pam_handle_t *pamh = NULL;

	struct pam_conv conv = {conv_function, (char *)password};

	retval = pam_start("login-pam-example", username, &conv, &pamh);

	if (retval == PAM_SUCCESS)
	{
		retval = pam_authenticate(pamh, 0);
	}

	if (retval == PAM_SUCCESS)
	{
		retval = pam_acct_mgmt(pamh, 0);
	}

	if (retval == PAM_SUCCESS)
	{
		printf("login successful \n");
	}

	else
	{
		printf("auth failed: %s\n", pam_strerror(pamh, retval));
	}

	if (pam_end(pamh, retval) != PAM_SUCCESS)
	{
		pamh = NULL;
		printf("failed to release pam handle\n");
		return 1;
	}

	return (retval == PAM_SUCCESS ? 0 : 1);
}
			

int main(void)
{
	int status;

	char user[256];
	char pass[256];

	printf("User name: ");
	scanf("%255s", user);

	printf("password: ");
	scanf("%255s", pass);

	status = login(user, pass);

	return status;
}
