import boolcalc
import telebot
from telebot import types
bot = telebot.TeleBot("7908905794:AAEa1uuoorYLkkN14mZ1E8K7GWh5qWRX5t4")
@bot.message_handler(commands=['start'])
def handle_start(message):
    markup = types.ReplyKeyboardMarkup()
    btn1 = types.KeyboardButton('Таблица')
    btn2 = types.KeyboardButton('Полином Жегалкина')
    btn3 = types.KeyboardButton('Проверка на полноту')
    markup.row(btn1, btn2, btn3)
    btn4 = types.KeyboardButton('Справка')
    btn5 = types.KeyboardButton('Очистка системы')
    btn6 = types.KeyboardButton('Отобразить систему')
    btn7 = types.KeyboardButton('Добавить или удалить выражение')
    markup.row(btn4,btn5,btn6)
    markup.row(btn7)
    welcome_message = (
        "Привет! Я бот для работы с булевыми выражениями.\n\n"
        "<b>Переменные в выражении:</b>\n"
        "Используйте формат <code>xN</code>, где N — натуральное число от 0 до 9 (например, <code>x1</code>, <code>x2</code>, <code>x3</code>, ...).\n"
        "Переменные могут принимать значения 0 или 1.\n\n"
        "<b>Поддерживаемые операции:</b>\n"
        "<code>~</code>   отрицание\n"
        "<code>&amp;</code>   конъюнкция\n"
        "<code>v</code>   дизъюнкция\n"
        "<code>+</code>   сложение по модулю 2\n"
        "<code>&gt;</code>   импликация (x → y)\n"
        "<code>&lt;</code>   обратная импликация (x ← y)\n"
        "<code>=</code>   эквиваленция (x ≡ y)\n"
        "<code>|</code>   штрих Шеффера (x | y)\n"
        "<code>^</code>   стрелка Пирса (x ↓ y)\n\n"
    )
    bot.send_message(message.chat.id, welcome_message, reply_markup=markup, parse_mode='HTML')

@bot.message_handler()
def on_click(message):
    try:
        if message.text == 'Таблица':
            result_table = boolcalc.process_expression("-table",message.chat.id).split('\n')
            table = boolcalc.process_expression("-FullTable",message.chat.id)
            bot.send_message(message.chat.id, f"Table results: \n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(result_table) if expr])}\n\n{table}")
            print(f"Table results:\n{result_table} \n{table}")
        elif message.text == 'Полином Жегалкина':
            result_zh = boolcalc.process_expression("-zh", message.chat.id).split('\n')
            bot.send_message(message.chat.id, f"Zhegalkin results:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(result_zh) if expr])}")
            print(f"Zhegalkin results:\n{result_zh}")
        elif message.text == 'Проверка на полноту':
            is_full = boolcalc.process_expression("-isfull", message.chat.id)
            bot.send_message(message.chat.id, f"Is full system: {is_full}")
            print(f"Is full system: {is_full}")
        elif message.text == 'Справка':
            msg = bot.send_message(message.chat.id,"<b>Справка по боту для работы с булевыми выражениями:</b>\n\n",parse_mode='HTML')
            help(msg)
        elif message.text == 'Очистка системы':
            msg = bot.send_message(message.chat.id,"<b>Список очищен.</b>\n\n",parse_mode='HTML')
            clear(msg)
        elif message.text == 'Отобразить систему':
            msg = bot.send_message(message.chat.id,"<b>Система: </b>\n\n",parse_mode='HTML')
            open_lines(msg)
        elif message.text == 'Добавить или удалить выражение':
            get_Expression(message)
    except Exception as e:
        error_message = f"Произошла ошибка: {e}"
        bot.send_message(message.chat.id, error_message)
        print(error_message)

def help(message):
    help_text = (
        "<b>Переменные в выражении:</b>\n"
        "Используйте формат <code>xN</code>, где N — натуральное число от 0 до 9 (например, <code>x1</code>, <code>x2</code>, <code>x3</code>, ...).\n"
        "Переменные могут принимать значения 0 или 1.\n\n"
        "<b>Поддерживаемые операции:</b>\n"
        "<code>~</code>   отрицание\n"
        "<code>&amp;</code>   конъюнкция\n"
        "<code>v</code>   дизъюнкция\n"
        "<code>+</code>   сложение по модулю 2\n"
        "<code>&gt;</code>   импликация (x → y)\n"
        "<code>&lt;</code>   обратная импликация (x ← y)\n"
        "<code>=</code>   эквиваленция (x ≡ y)\n"
        "<code>|</code>   штрих Шеффера (x | y)\n"
        "<code>^</code>   стрелка Пирса (x ↓ y)\n\n"
        "<b>Действия с выражениями (через кнопки):</b>\n"
        "<b>Таблица</b>: Построить таблицу истинности для всех добавленных выражений.\n"
        "<b>Полином Жегалкина</b>: Построить полином Жегалкина для всех добавленных выражений.\n"
        "<b>Проверка на полноту</b>: Проверить систему добавленных функций на полноту.\n\n"
    )
    bot.send_message(message.chat.id, help_text, parse_mode='HTML')

def clear(message):
    boolcalc.clear_expressions(message.chat.id)
def open_lines(message):
    current_user_lines = boolcalc.print_expressions(message.chat.id)
    if current_user_lines:
        bot.send_message(message.chat.id, current_user_lines)
    else:
        bot.send_message(message.chat.id, "Список выражений пуст.")

def get_Expression(message):
    markup = types.InlineKeyboardMarkup()
    btn1 = types.InlineKeyboardButton('Добавить выражение',callback_data='Add')
    btn2 = types.InlineKeyboardButton('Удалить выражение', callback_data='Delete')
    markup.row(btn1,btn2)
    bot.send_message(message.chat.id,'Выберите действие:',reply_markup=markup)

@bot.callback_query_handler(func=lambda callback: True)
def callback_massage(callback):
    if callback.data == 'Add':
        msg = bot.send_message(callback.message.chat.id, 'Введите булевое выражение:')
        bot.register_next_step_handler(msg ,add_expresion)
        bot.delete_message(callback.message.chat.id, callback.message.message_id)
    elif callback.data == 'Delete':
        if not boolcalc.empty_expressions(callback.message.chat.id):
            expressions = boolcalc.print_expressions(callback.message.chat.id)
            msg = bot.send_message(callback.message.chat.id, f"Текущие выражения:\n{expressions}\n\nВведите номер выражения для удаления:")
            bot.register_next_step_handler(msg ,delete_expresion)
            bot.delete_message(callback.message.chat.id, callback.message.message_id)
        else:
            bot.send_message(callback.message.chat.id, "Список выражений пуст.")

def add_expresion(message):
    try:
        boolcalc.add_expression(message.chat.id, message.text)
        bot.send_message(message.chat.id,f"Выражение '{message.text}' добавлено в список.")
    except Exception as e:
        error_message = f"Не удалось добвить выражение. Ошибка: {e}"
        bot.send_message(message.chat.id, error_message)
def delete_expresion(message):
    try:
        index = int(message.text) - 1
        deleted = boolcalc.delete_expression(message.chat.id,index)
        bot.send_message(message.chat.id, f"Выражение '{deleted}' удалено.")
    except ValueError as e:
        bot.send_message(message.chat.id, f"Не удалось удалить выражение. Ошибка: {e}")

if __name__ == '__main__':
    print("Бот запускается...")
    bot.polling(non_stop=True)
