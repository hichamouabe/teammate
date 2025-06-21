/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aelbouz <aelbouz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 04:51:41 by houabell          #+#    #+#             */
/*   Updated: 2025/06/21 08:27:45 by aelbouz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <dirent.h>
# include <errno.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <stdbool.h>
# include <limits.h>

# define SUCCESS 0
# define ERROR 1
# define MAX_HEREDOCS 16

typedef enum e_quote_type {
	QUOTE_NONE,
	QUOTE_SINGLE,
	QUOTE_DOUBLE
}	t_quote_type;

typedef enum e_token_type {
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct s_token {
	t_token_type	type;
	char			*value;
	int				is_heredoc_delimiter_value;
	int				original_delimiter_had_quotes;
	int				is_from_redir;
	struct s_token	*next;
}	t_token;

typedef struct s_env {
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_var_info {
	char				*name;
	size_t				length;
	t_quote_type		quote_context;
	struct s_var_info	*next;
}	t_var_info;

typedef struct s_redirect {
	t_token_type		type;
	char				*file;
	struct s_redirect	*next;
}	t_redirect;

typedef struct s_command {
	char				**args;
	t_redirect			*redirects;
	int					fd[2];
	int					file;
	struct s_command	*next;
}	t_command;

typedef struct s_execution_info
{
	int		i;
	int		cmd_count;
	int		status;
	int		stdout_save;
	int		stdin_save;
	t_env	**env;
}	t_execution_info;

typedef struct s_shell {
	t_token			*tokens;
	t_command		*commands;
	t_env			*env;
	int				exit_status;
	int				running;
	int				heredoc_sigint;
	char			*input;
	t_var_info		*variables;
	t_quote_type	csqt;
	char			**heredoc_files;
	int				heredoc_count;
}	t_shell;

typedef struct s_quote_params {
	char	*input;
	int		*i;
	int		*start;
	char	**word;
	t_shell	*shell;
}	t_quote_params;

typedef struct s_word_context {
	char	*word;
	t_token	*token;
	t_shell	*shell;
	int		expect_delimiter;
}	t_word_context;

typedef struct s_expansion_state
{
	char	**result;
	char	*buffer;
	int		i;
}	t_expansion_state;

typedef struct s_word_state {
	char	*input;
	int		start_word;
	int		start_seg;
	int		*i;
}	t_word_state;

extern volatile int	g_signal_status;
// Shell Core (src/shell)
t_shell			*init_shell(char **envp);
void			free_shell(t_shell *shell);
void			reset_shell(t_shell *shell);
int				minishell_loop(t_shell *shell);
int				validate_syntax(t_token *tokens);
void			free_var_info_list(t_var_info *var_head);
void			execute_commands(t_shell *shell);
int				check_pipe_syntax(t_token *tokens);
int				check_redirection_syntax(t_token *tokens);
void			free_cmd(t_command *cmd);
int				check_ambig(t_shell *shell);
void			cleanup_files(t_shell *shell);
// Tokenization (src/tokenization)
t_token			*tokenize(char *input, t_shell *shell);
t_token			*new_token(t_token_type type, char *value);
void			add_token(t_token **tokens, t_token *new);
void			free_tokens(t_token *tokens);
int				handle_quotes(char *input, int *i, char **value);
t_token			*handle_special(char *input, int *i);
t_token			*handle_word(char *input, int *i, \
				t_shell *shell, int expect_deli);
int				is_special(char c);
int				is_whitespace(char c);
int				is_redirection(t_token_type type);
int				ft_isalnum(char c);
char			*append_str(char *str, char *append);
int				append_segment_to_word(char *input, int start, \
				int current_i, char **word);

void			print_tokens(t_token *tokens);

// Expansion (src/expansion)
t_command		*parse_tokens(t_shell *shell);
void			expand_variables(t_shell *shell);
void			create_commands(t_shell *shell);
char			**expand_token_value(char *str, \
				t_var_info **var_list, t_shell *shell);
void			process_regular_text(char *str, t_expansion_state *state);
char			*extract_variable_name(char *str, t_expansion_state *state, \
				t_var_info **var_list);
void			skip_regular_text(char *str, t_expansion_state *state);
void			process_variable_expansion(char *str, t_expansion_state *state, \
				t_var_info **var_list, t_shell *shell);
void			apply_expansion_by_context(t_expansion_state *state, \
				char *expanded_value, t_var_info **var_list);
void			handle_single_quote_context(t_expansion_state *state, \
				char *expanded_value, t_var_info **var_list);
void			handle_double_quote_context(t_expansion_state \
				*state, char *expanded_value);
void			handle_unquoted_context(t_expansion_state *state, \
				char *expanded_value);
void			process_word_splitting(t_expansion_state *state, \
				char *expanded);
void			distribute_split_words(t_expansion_state *state, char **split);
void			flush_buffer_to_result(t_expansion_state *state);
void			append_segment(char **buffer, char *part);
char			*get_env_value(char *name, t_shell *shell);
char			**ensure_valid_result(char **result);
void			add_segment(char ***result, char *segment);
void			copy_existing_segments(char **new_arr, \
				char **old_arr, int count);
void			cleanup_expansion_step(char *var_name, char *expanded_value, \
				t_var_info **var_list);
void			handle_extraction_failure(t_expansion_state *state, \
				t_var_info **var_list);
void			handle_expansion_failure(t_expansion_state *state, \
				char *var_name);
int				has_variable(t_token *token);
void			process_each_token(t_token **token, t_var_info **var, \
				t_shell *shell, int is_from_redir);
t_token			*create_token_list(char **segments, int is_from_redir);
t_token			*get_last_token(t_token *token_list);
void			replace_first_token(t_token *original, t_token *new_first);
void			insert_remaining_tokens(t_token *original, t_token *new_list);
char			*extract_var_name(char *str_after_dol, int *name_len);
void			handle_question_mark(t_shell *shell, int *index);
void			process_variable(t_shell *shell, char *segment, int *index);
void			log_expandable(t_shell *shell, char *segment);
t_var_info		*create_var_node(char *name, t_quote_type context_csqt);
void			add_var_node(t_var_info **var_head, t_var_info *new_node);
void			free_var(t_var_info *var_head);
int				handle_quoted_part(t_quote_params *params, int expect_deli);
int				handle_quote_in_word(char *input, int *i, \
				int *start, t_word_context *ctx);
void			init_word_context(t_word_context *ctx, t_shell *shell);
t_token			*finalize_word(char *input, int start, \
				int *i, t_word_context *ctx);
int				segment_has_any_actual_quotes(const char *segment_str, int len);

void			print_variables(t_var_info *variables);

// Environment Handling (src/env_handling)
t_env			*init_env(char **envp);
void			free_env(t_env *env);
t_env			*new_env_node(char *key, char *value);
void			add_env_node(t_env **env, t_env *new);
t_env			*get_env_node(t_env *env, char *key);

// Signal Handling (src/signal_handling)
void			handle_signals(void);
void			sigint_handler(int sig);
void			sigquit_handler(int sig);
void			sigint_heredoc_handler(int sig);

// Utilities (src/utils)
size_t			ft_strlen(const char *s);
char			*ft_strdup(const char *s1);
char			*ft_strndup(const char *s1, size_t n);
char			*ft_strchr(const char *s, int c);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_substr(char const *s, unsigned int start, size_t len);
char			*ft_itoa(int n);
char			**ft_split(char const *s, char c);
char			*ft_strjoin(char const *s1, char const *s2);
int				ft_isalpha(int c);
size_t			ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t			ft_strlcpy(char *dst, const char *src, size_t destsize);
int				is_allowed(char c);
void			free_arr(char **array);
char			*ft_strcpy(char *dst, const char *src);
char			*ft_strcat(char *dst, const char *src);
void			*ft_memcpy(void *dst, const void *src, size_t n);

// Heredoc functions
char			*expand_heredoc_line(char *line, t_shell *shell);
char			*generate_heredoc_filename(t_shell *shell);
int				handle_heredocs(t_shell *shell);
int				read_heredoc_input(char *delimiter, int expand, t_shell *shell);

//exucution functions
int				is_builtin(char *cmd, char **args, char *env_path, t_env **env);
int				ft_echo(char **av);
int				ft_pwd(t_env **env);
int				ft_exit(char **args);
char			*find_executable(char *cmd, char *path_env);
int				is_not_builtin(char **args, char *env_path, t_env *env);
char			**env_to_array(t_env *env);
// void			init_env(char **envp);
void			free_env(t_env *env);
t_env			*get_env(t_env *new_env);
int				ft_cd(char **args, t_env **env);
int				ft_export(char **args, t_env **env);
void			updat_env(t_env **env, char *key, char *value);
int				find_and_update(t_env **env, char *key, char *value);
t_env			*env_sorted(t_env *env);
int				handl_export_args(char *arg, t_env **env);
t_env			*copie_env(t_env	*head);
int				is_valide_args(char *key);
int				ft_unset(char **args, t_env **env);
int				execute_with_setup(t_command **cmds, t_command *cmd, \
				t_execution_info *info, char *env_path);
int				cleanup_execution(t_execution_info *info);
int				handle_redir(t_command *cmd);
int				execute_multiple_commands(t_command **cmds, t_env **env, \
				int cmd_count, t_execution_info *info);
int				execute_single_command(t_command *cmd, t_env **env);
void			handl_plus(t_env **env, char *key, char *value);
int				setup_io(t_execution_info *info);
char			*get_my_env(char *name, t_env *env);
t_env			*init_default_env(t_env **env);
int				handle_out_redir(t_redirect *redir_info);
void			ft_lstadd_back(t_env **lst, t_env *new);
void			ft_putstr_fd(char *s, int fd);
int				ft_atoi(const char *str);
int				ft_isdigit(char c);
char			*ft_strtrim(char const *s1, char const *set);
int				ft_lstsize(t_env *lst);
void			close_fds(t_command *cmd, int stdout_save, int stdin_save);
void			free_cmd(t_command *cmd);
t_command		*parse_single_command(t_token **tokens);
void			add_redirect(t_command *cmd, t_token_type type, char *filename);
void			count_args_and_redirs(t_token *start, int *arg_c, int *redir_c);
int				env_to_array2(char **envp, int	*i, t_env *tmp);

#endif
