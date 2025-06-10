import boolcalc
import telebot
from telebot import types
import io
from PIL import Image, ImageDraw, ImageFont
import csv
bot = telebot.TeleBot("7908905794:AAEa1uuoorYLkkN14mZ1E8K7GWh5qWRX5t4")
@bot.message_handler(commands=['start'])
def handle_start(message):
    markup = types.ReplyKeyboardMarkup()
    btn1 = types.KeyboardButton('Таблица')
    btn2 = types.KeyboardButton('Полином Жегалкина')
    btn3 = types.KeyboardButton('Проверка на полноту')
    btn8 = types.KeyboardButton('СКНФ')
    btn9 = types.KeyboardButton('СДНФ')
    markup.row(btn1, btn2, btn3, btn8, btn9)
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
        "<b>Примечание!!!!!!:</b> Если в выражений различных переменных больше 6, то таблица истинности будет выводиться ввиде CSV файла."
    )
    bot.send_message(message.chat.id, welcome_message, reply_markup=markup, parse_mode='HTML')
def table_to_csv(table_text):
    lines = table_text.strip().split('\n')

    csv_buffer = io.StringIO()
    csv_writer = csv.writer(csv_buffer)

    for line in lines:
        row = [cell for cell in line.split(' ') if cell]
        csv_writer.writerow(row)

    byte_buffer = io.BytesIO()
    byte_buffer.write(csv_buffer.getvalue().encode('utf-8'))
    byte_buffer.seek(0)

    return byte_buffer
def text_to_image(text, width=800, height=None, bg_color=(255, 255, 255)):
    lines = text.count('\n') + 1
    if height is None:
        height = max(300, lines * 25)

    image = Image.new("RGB", (width, height), bg_color)
    draw = ImageDraw.Draw(image)

    try:
        font = ImageFont.truetype("DejaVuSans.ttf", 16)
    except:
        font = ImageFont.load_default()

    draw.text((20, 20), text, fill=(0, 0, 0), font=font)

    bio = io.BytesIO()
    bio.name = 'result.png'
    image.save(bio, 'PNG')
    bio.seek(0)

    return bio
@bot.message_handler()
def on_click(message):
    try:
        if message.text == 'Таблица':
            result_table = boolcalc.process_expression("-table",message.chat.id).split('\n')
            print(len(result_table[0]))
            bot.send_message(message.chat.id, f"Вектор функция: \n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(result_table) if expr])}")
            table = boolcalc.process_expression("-FullTable",message.chat.id).split('=')
            i = 0
            j = 0
            for expr in table:
                if expr == '' or expr == '\n':
                    continue
                if len(result_table[j]) > 64:
                    csv_file = table_to_csv(expr)
                    bot.send_document(message.chat.id, csv_file, caption=f"Таблица истинности для {boolcalc.get_expression(message.chat.id,j)} в формате CSV", visible_file_name=f"{i+1}truth_table.csv")
                    i += 1
                else:
                    text = f"Таблица истинности для {boolcalc.get_expression(message.chat.id,j)}: \n{expr}"
                    img = text_to_image(text)
                    bot.send_photo(message.chat.id, photo=img)
                j += 1
            print(f"Table results:\n{result_table} \n{table}")
        elif message.text == 'Полином Жегалкина':
            result_zh = boolcalc.process_expression("-zh", message.chat.id).split('\n')
            bot.send_message(message.chat.id, f"Жегалкин:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(result_zh) if expr])}")
            print(f"Zhegalkin results:\n{result_zh}")
        elif message.text == 'Проверка на полноту':
            is_full = boolcalc.process_expression("-isfull", message.chat.id)
            bot.send_message(message.chat.id, f"Полнота системы: {is_full}")
            print(f"Is full system: {is_full}")
        elif message.text == 'СКНФ':
            sknf = boolcalc.process_expression("-sknf", message.chat.id).split('\n')
            bot.send_message(message.chat.id, f"СКНФ:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(sknf) if expr])}")
            print(f"Is full system: {sknf}")
        elif message.text == 'СДНФ':
            sdnf = boolcalc.process_expression("-sdnf", message.chat.id).split('\n')
            bot.send_message(message.chat.id, f"СДНФ:\n{'\n'.join([f'{i+1}. {expr}' for i, expr in enumerate(sdnf) if expr])}")
            print(f"Is full system: {sdnf}")
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
        "<b>Примечание!!!!!!:</b> Если в выражений различных переменных больше 6, то таблица истинности будет выводиться ввиде CSV файла."
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
        error_message = f"Не удалось добавить выражение. Ошибка: {e}"
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
