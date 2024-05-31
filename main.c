#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <security/pam_modules.h>

static int conv_function(int num_msg, const struct pam_message **msg, struct pam_response **resp,
			void *appdata_ptr)
{
	printf("pam conv function");

	return PAM_SUCCESS;
}

int main(void)
{
	pam_handle_t *pamh = NULL;

	int retval;
	const char *username;
	char user[256];
	// char pass[256];

	printf("User name: ");
	scanf("%255s", user);
	username = user;

	const char *pass[256] = {"i", "s", "a", "a", "c", "w","e","s","t","a","w","a","y", "\0"};

	const char *data[2] = {user, *pass};
	struct pam_conv conv = {conv_function, data};

	retval = pam_start("login-pam-example", username, &conv, &pamh);
	
	if (retval == PAM_SUCCESS)
	{
		retval = pam_set_item(pamh, PAM_AUTHTOK, (const char *)pass);
	}

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
		printf("login successful\n");
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
